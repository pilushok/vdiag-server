// project headers
// #include "can.h"
#include "can.h"
#include "logger.h"
#include <stdio.h>
#include <unistd.h>

#define INTERFACE_NAME "vcan0"
#define WELCOME_MESSAGE  ("Virtual UDS Server v0.0.0\n\
Powered by Nikita Piliugin\n\
Simulating ISO 14229 over CAN (UDS)\n")

int main(int argc, char **argv)
{
  uint8_t ubuf[4096];
  enum can_error can_err = CAN_ERROR_COMMON;
  printf(WELCOME_MESSAGE);
  
  can_err = can_socket_open(INTERFACE_NAME, 0x100, 0x120);
  if (can_err != CAN_NO_ERROR) {
    return -1;
  }

  while (1) {
      //can_err can_socket_read(nt32_t sockfd, ubuf)
      // if (can_err != CAN_NO_ERROR)
      //   printf("error happened");
      
      // uds_handle_msg(ubuf);

      usleep(10000);
  }

  // can_socket_close(struct socket_state **ppsock_state)
  return 0;
}
