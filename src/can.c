#include "can.h"
#include "linux/can.h"

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void print_hex(const uint8_t *pdata, uint32_t usz) {
    for (size_t i = 0; i < usz; i++) {
        printf("%02X ", pdata[i]);
    }
    printf("\n");
}

static enum can_error set_nonblocking(int32_t sockfd) {
    int32_t flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl(F_GETFL)");
        return CAN_ERROR_IOCTL;
    }

    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl(F_SETFL)");
        return CAN_ERROR_IOCTL;
    }

    return 0;
}

struct socket_state *can_socket_open(const char *ifname, const uint32_t utx,
                                     const uint32_t urx, enum can_error *perr) {
    uint8_t usockf = 0;
    int32_t idx = 0;
    struct socket_state *psock_state = NULL;

    psock_state = (struct socket_state *)calloc(1, sizeof(struct socket_state));
    if (NULL == psock_state) {
        perror("malloc");
        *perr = CAN_ERROR_MALLOC;
        goto err;
    }

    // TODO: here we can put support of other tp
    psock_state->sockfd = socket(PF_CAN, SOCK_DGRAM, CAN_ISOTP);
    if (psock_state->sockfd < 0) {
        perror("connect");
        *perr = CAN_ERROR_SOCKET_INIT;
        goto err;
    }
    psock_state->proto = CAN_ISOTP;
    usockf = 1;

    memset(&psock_state->addr, 0, sizeof(psock_state->addr));
    psock_state->addr.can_family = AF_CAN;
    psock_state->addr.can_ifindex = if_nametoindex(ifname);
    psock_state->addr.can_addr.tp.tx_id = utx;
    psock_state->addr.can_addr.tp.rx_id = urx;

    if (CAN_NO_ERROR != set_nonblocking(psock_state->sockfd)) {
        perror("fcntl");
        goto err;
    }
    if (bind(psock_state->sockfd, (struct sockaddr *)&psock_state->addr,
             sizeof(psock_state->addr))) {
        perror("bind");
        *perr = CAN_ERROR_BIND;
        goto err;
    }
    printf("Can connection established interface: %s, tx: %X, rx: %X\n", ifname,
           utx, urx);

    return psock_state;

err:
    if (psock_state)
        free(psock_state);

    if (usockf)
        close(psock_state->sockfd);

    return NULL;
}

void can_socket_close(struct socket_state **ppsock_state) {
    if (!ppsock_state || !*ppsock_state)
        return;

    close((*ppsock_state)->sockfd);
    free(*ppsock_state);
    *ppsock_state = NULL;
}

enum can_error can_socket_read(struct socket_state *psock_state,
                               struct can_message *pmsg) {
    uint32_t usz_io = 0;
    int32_t irc = -1;
    fd_set read_fds;

    if (!psock_state || !pmsg || !pmsg->usz_max) {
        return CAN_ERROR_INVALID_PARAM;
    }

    if (psock_state->sockfd <= 0) {
        return CAN_ERROR_SOCKET_CLOSED;
    }

    FD_ZERO(&read_fds);
    FD_SET(psock_state->sockfd, &read_fds);

    struct timeval tv = {.tv_sec = psock_state->uioto / 1000,
                         .tv_usec = (psock_state->uioto % 1000) * 1000};

    irc = select(psock_state->sockfd + 1, &read_fds, NULL, NULL, &tv);

    if (irc == 0) {
        printf("select timeout\n");
        return CAN_ERROR_TIMEOUT;
    } else if (irc < 0) {
        perror("select");
        return CAN_ERROR_READ;
    }

    usz_io = read(psock_state->sockfd, pmsg->pdata, pmsg->usz_max);

    if (usz_io < 0) {
        perror("read");
        return CAN_ERROR_READ;
    } else if (usz_io == 0) {
        printf("Can socket was not openned yet\n");
        return CAN_ERROR_SOCKET_CLOSED;
    }

    printf("Can message received hex: ");
    print_hex(pmsg->pdata, usz_io);
    pmsg->usz_io = usz_io;
    return CAN_NO_ERROR;
}

enum can_error can_socket_write(struct socket_state *psock_state,
                                const struct can_message *pmsg) {
    return CAN_NO_ERROR;
}
