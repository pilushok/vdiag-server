#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/uio.h>

#include <stdint.h>
#include "linux/can.h"
#include "linux/can/raw.h"
#include "net/if.h"
#include "string.h"
#include "sys/socket.h"

// Different max
#define MAX_SLOTS 2
#define MAX_MESSAGE_SIZE 4096

enum can_error {
    CAN_NO_ERROR = 0,
    CAN_ERROR_COMMON = 1,
    CAN_ERROR_SOCKET_INIT,
    CAN_ERROR_MALLOC,
    CAN_ERROR_CONNECT,
    CAN_ERROR_BIND,
    CAN_ERROR_INVALID_PARAM,
    CAN_ERROR_SOCKET_CLOSED,
    CAN_ERROR_TIMEOUT,
    CAN_ERROR_READ,
    CAN_ERROR_WRITE,
    CAN_ERROR_IOCTL,
};

struct can_message {
    uint8_t pdata[MAX_MESSAGE_SIZE];
    uint32_t usz_max;
    uint32_t usz_io;
};

struct socket_state {
    int32_t sockfd;
    int32_t proto;
    uint32_t uioto;
    struct sockaddr_can addr;
};

struct socket_state *can_socket_open(const char *ifname, const uint32_t utx,
                                     const uint32_t urx, enum can_error *perr);

void can_socket_close(struct socket_state **ppsock_state);

enum can_error can_socket_read(struct socket_state *psock_state,
                               struct can_message *pmsg);

enum can_error can_socket_write(struct socket_state *psock_state,
                                const struct can_message *pmsg);
