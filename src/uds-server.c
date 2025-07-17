// project headers
// #include "can.h"
#include "can.h"
#include "logger.h"
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

#define CAN_TIMEOUT 5000
#define INTERFACE_NAME "vcan0"
#define WELCOME_MESSAGE ("Virtual UDS Server v0.0.0\n\
Powered by Nikita Piliugin\n\
Simulating ISO 14229 over CAN (UDS)\n")

int main(int argc, char **argv) {
    enum can_error err = CAN_ERROR_COMMON;
    struct socket_state *psock_state = NULL;
    struct can_message pmsg;
    printf(WELCOME_MESSAGE);

    psock_state = can_socket_open(INTERFACE_NAME, 0x100, 0x120, &err);
    if (NULL == psock_state) {
        return -1;
    }
    psock_state->uioto = CAN_TIMEOUT;

    pmsg.usz_max = MAX_MESSAGE_SIZE;
    while (1) {
        err = can_socket_read(psock_state, &pmsg);

        // uds_handle_msg(ubuf);

        usleep(10000);
    }

    // can_socket_close(struct socket_state **ppsock_state)
    return 0;
}
