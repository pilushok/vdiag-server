// #include "can.h"
#include "can.h"
#include "logger.h"
#include "uds.h"
#include "uds_def.h"
#include "ecu_config.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>

#define CAN_TIMEOUT 1000
#define CONFIGS_DEFAULT_DIR ".local/vds/state"
#define WELCOME_MESSAGE                   \
    ("Virtual Diagnostic Server v1.0.0\n" \
     "Powered by Nikita Piliugin\n"       \
     "Simulating ISO 14229 over CAN (UDS)\n")

// Global variables
static struct socket_state *psock_state = NULL;
static uds_state_t         *pstate      = NULL;

static const char *get_home()
{
    const char *home = getenv("HOME");
    if (home != NULL && home[0] != '\0') {
        return home;
    }
    return NULL;
}

void uds_server_stop()
{
    can_socket_close(&psock_state);
    uds_deinit(&pstate);
}

void signal_handler(int sig)
{
    printf("\nReceived signal %d, shutting down gracefully...\n", sig);
    uds_server_stop();
    exit(sig);
}

int main(int argc, char **argv)
{
    enum can_error     err = CAN_ERROR_COMMON;
    struct can_message req, resp;
    uds_error_t        uds_err = UDS_ERROR_HANDLER_INIT;
    uint32_t           urx, utx;
    ecu_config_t       ecucfg;
    int32_t            ierr;
    const char        *chome;
    char               cfgdir[256];
    chome = get_home();
    if (chome == NULL) {
        printf("failed to read HOME variable from env\n");
    }
    snprintf(cfgdir, sizeof(cfgdir), "%s/%s", chome, CONFIGS_DEFAULT_DIR);

    if (argc < 2) {
        printf("Invalid uds_server call. Try uds_server "
               "<absolute_path_to_config>\n");
        return EXIT_FAILURE;
    }

    ierr = parse_config(argv[1], &ecucfg);
    if (ierr < 0) {
        printf("Error parsing config from file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    ierr = create_cfgdir(cfgdir);
    if (ierr < 0) {
        printf("Unable to create configs directory %s\n", cfgdir);
        return EXIT_FAILURE;
    }

    ierr = create_memfile(ecucfg.memory.file_path,
                          ecucfg.memory.end_addr - ecucfg.memory.start_addr);
    if (ierr < 0) {
        printf("failed to create ecu memory file\n");
        return EXIT_FAILURE;
    }

    printf(WELCOME_MESSAGE);
    // TODO: need to implement and validation of addresses
    psock_state = can_socket_open(ecucfg.can.interface, ecucfg.can.tx_id,
                                  ecucfg.can.rx_id, &err);
    if (!psock_state) {
        return EXIT_FAILURE;
    }
    psock_state->uioto = CAN_TIMEOUT;

    pstate = uds_init(ecucfg.uds.cimpl_libname, &uds_err);
    if (!pstate) {
        printf("Unable to init uds_state ret_code: %d", uds_err);
        return EXIT_FAILURE;
    }
    pstate->pecucfg = &ecucfg;

    // setup signal handler for correct shuting down
    signal(SIGINT, signal_handler);
    signal(SIGINT, signal_handler);

    while (1) {
        memset(&req, 0, sizeof(req));
        memset(&resp, 0, sizeof(resp));
        err = can_socket_read(psock_state, &req);
        if (CAN_NO_ERROR == err) {
            uds_err = uds_handle_msg(pstate, req, &resp);
            if (uds_err == UDS_NO_ERROR) {
                err = can_socket_write(psock_state, &resp);
            }
        }

        usleep(10000);
    }

    return 0;
}
