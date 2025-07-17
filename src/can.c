#include "can.h"
#include "linux/can.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


struct socket_state *can_socket_open(const char *ifname,
                               const uint32_t utx,
                               const uint32_t urx,
                               enum can_error *perr) {
    uint8_t usockf = 0;
    int32_t idx = 0;
    struct socket_state *psock_state = NULL;
    struct sockaddr_can addr;

    psock_state = (struct socket_state *) calloc(1, sizeof(struct socket_state));
    if (NULL == psock_state) {
        perror("malloc");
        *perr = CAN_ERROR_MALLOC;
        goto err;
    }

    // TODO: here we can put support of other tp
    psock_state->socket_fd = socket(PF_CAN, SOCK_DGRAM, CAN_ISOTP);
    if (psock_state->socket_fd < 0) {
        perror("connect");
        *perr = CAN_ERROR_SOCKET_INIT;
        goto err;
    }
    usockf = 1;

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = if_nametoindex(ifname);
    addr.can_addr.tp.tx_id = utx;
    addr.can_addr.tp.rx_id = urx;

    if (bind(psock_state->socket_fd, (struct sockaddr *) &addr, sizeof(addr))) {
        perror("bind");
        *perr = CAN_ERROR_BIND;
        goto err;
    }
    printf("Can connection established interface: %s, tx: %X, rx: %X\n", ifname,
           utx, urx);

    return psock_state;

err:
    if (psock_state) free(psock_state);

    if (usockf) close(psock_state->socket_fd);
  
    return NULL;
}

void can_socket_close(struct socket_state **ppsock_state) {
    if (!ppsock_state || !*ppsock_state)
        return;

    close((*ppsock_state)->socket_fd);
    free(*ppsock_state);
    *ppsock_state = NULL;
}

enum can_error can_socket_read(struct socket_state *psock_state, struct can_frame *frame) {
    return CAN_NO_ERROR;
}

enum can_error can_socket_write(struct socket_state *psock_state, const struct can_frame *frame) {
    return CAN_NO_ERROR;
}
