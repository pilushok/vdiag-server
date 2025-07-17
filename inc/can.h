#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/uio.h>

#include <stdint.h>
#include "linux/can.h"
#include "linux/can/raw.h"
#include "net/if.h"
#include "string.h"
#include "sys/socket.h"

#define MAX_SLOTS 2

enum can_error {
  CAN_NO_ERROR = 0,
  CAN_ERROR_COMMON,
  CAN_ERROR_SOCKET_INIT,
  CAN_ERROR_MALLOC,
  CAN_ERROR_CONNECT,
  CAN_ERROR_BIND,
};


struct socket_state {
  int32_t socket_fd;
  int32_t proto;
  struct sockaddr_can addr;
};

struct socket_state *can_socket_open(const char *ifname,
    const uint32_t utx,
    const uint32_t urx,
    enum can_error *perr);

void can_socket_close(struct socket_state **ppsock_state);

enum can_error can_socket_read(struct socket_state *psock_state, struct can_frame *frame);

enum can_error can_socket_write(struct socket_state *psock_state, const struct can_frame *frame);

