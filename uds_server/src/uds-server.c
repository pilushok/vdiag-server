// project headers
// #include "can.h"
#include "can.h"
#include "logger.h"
#include "uds.h"
#include "uds_def.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#define CAN_TIMEOUT 1000
#define INTERFACE_NAME "vcan0"
#define WELCOME_MESSAGE             \
    ("Virtual UDS Server v0.0.0\n"  \
     "Powered by Nikita Piliugin\n" \
     "Simulating ISO 14229 over CAN (UDS)\n")

int main(int argc, char **argv)
{
    enum can_error       err         = CAN_ERROR_COMMON;
    struct socket_state *psock_state = NULL;
    struct can_message   msg;

    uds_state_t *pstate  = NULL;
    uds_error_t  uds_err = UDS_ERROR_HANDLER_INIT;

    if (argc < 2) {
        printf("Invalid uds_server call. Try uds_server <handlers_lib_path>\n");
        return -1;
    }

    printf(WELCOME_MESSAGE);
    psock_state = can_socket_open(INTERFACE_NAME, 0x100, 0x120, &err);
    if (NULL == psock_state) {
        return -1;
    }
    psock_state->uioto = CAN_TIMEOUT;

    pstate = uds_init(argv[1], &uds_err);
    if (!pstate) {
        return -1;
    }

    msg.usz_max = MAX_MESSAGE_SIZE;
    while (1) {
        err = can_socket_read(psock_state, &msg);
        if (CAN_NO_ERROR == err) {
            msg = (struct can_message){
                .usz_io = 1, .pdata = {0x00}, .usz_max = MAX_MESSAGE_SIZE};
            err = can_socket_write(psock_state, &msg);
        }
        // uds_handle_msg(ubuf);

        usleep(10000);
    }

    // can_socket_close(struct socket_state **ppsock_state)
    return 0;
}
