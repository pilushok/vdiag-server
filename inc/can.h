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
};

enum can_error can_socket_open(const char *ifname,
    const uint32_t utx,
    const uint32_t urx);

void can_socket_close(int32_t sockfd);

enum can_error can_socket_read(int32_t sockfd, struct can_frame *frame);

enum can_error can_socket_write(int32_t sockfd, const struct can_frame *frame);

