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
#define INTERFACE_NAME "vcan0"
#define WELCOME_MESSAGE             \
    ("Virtual UDS Server v0.0.0\n"  \
     "Powered by Nikita Piliugin\n" \
     "Simulating ISO 14229 over CAN (UDS)\n")

// Global variables
static struct socket_state *psock_state = NULL;
static uds_state_t         *pstate      = NULL;

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

    // if (argc < 4) {
    //     printf("Invalid uds_server call. Try uds_server <handlers_lib_path> "
    //            "<can_tx_id> <can_rx_id>\n");
    //     return EXIT_FAILURE;
    // }
    int32_t perr = parse_config("configs/test.ini", &ecucfg);
    if (perr < 0) {
        printf("Parse error of config\n");
    }
    print_config(&ecucfg);

    printf(WELCOME_MESSAGE);
    // TODO: need to implement and validation of addresses
    psock_state =
        can_socket_open(INTERFACE_NAME, atoi(argv[2]), atoi(argv[3]), &err);
    if (!psock_state) {
        return EXIT_FAILURE;
    }
    psock_state->uioto = CAN_TIMEOUT;

    pstate = uds_init(argv[1], &uds_err);
    if (!pstate) {
        printf("Unable to init uds_state ret_code: %d", uds_err);
        return EXIT_FAILURE;
    }

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
