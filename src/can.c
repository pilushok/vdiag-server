#include "can.h"
#include "linux/can.h"
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>


static struct socket_state *g_slots[MAX_SLOTS];


enum can_error can_socket_open(const char *ifname,
                               const uint32_t utx,
                               const uint32_t urx)
{
  enum can_error err = CAN_ERROR_COMMON;
  int32_t idx = 0;
  struct socket_state *psock_state = NULL;
  struct sockaddr_can addr;
  while (idx++ < MAX_SLOTS) {
    if (g_slots[idx] == NULL) {
      psock_state = g_slots[idx];
      break;
    }
  }

  psock_state = (struct socket_state *) calloc(1, sizeof(struct socket_state)); 
  if (psock_state == NULL) {
    perror("malloc");
    return CAN_ERROR_MALLOC;
  }
  
  // TODO: here we can put support of other tp
  psock_state->socket_fd = socket(PF_CAN, SOCK_DGRAM, CAN_ISOTP);
  if (psock_state->socket_fd < 0) {
    perror("connect");
    return CAN_ERROR_CONNECT;
  }

  memset(&addr, 0, sizeof(addr));
  addr.can_family = AF_CAN;
  addr.can_ifindex = if_nametoindex(ifname);  
  addr.can_addr.tp.tx_id = utx; 
  addr.can_addr.tp.rx_id = urx;

  if (bind(psock_state->socket_fd, (struct sockaddr *) &addr, sizeof(addr))) {
    perror("bind");
    close(psock_state->socket_fd);
    free(psock_state);
    psock_state = NULL;
    return CAN_ERROR_BIND; 
  }
  printf("Can connection established interface: %s, tx: %X, rx: %X\n", ifname, utx, urx);

  return CAN_NO_ERROR;
}


void can_socket_close(struct socket_state **ppsock_state)
{
  if (!ppsock_state || !*ppsock_state)
    return;

  close((*ppsock_state)->socket_fd);
  free(*ppsock_state);
  *ppsock_state = NULL;
}


enum can_error can_socket_read(int32_t sockfd, struct can_frame *frame)
{
  return CAN_NO_ERROR;
}


enum can_error can_socket_write(int32_t sockfd, const struct can_frame *frame)
{
  return CAN_NO_ERROR;
}

