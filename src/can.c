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

  return CAN_NO_ERROR;
}


void can_socket_close(int32_t sockfd)
{
}


enum can_error can_socket_read(int32_t sockfd, struct can_frame *frame)
{
  return CAN_NO_ERROR;
}


enum can_error can_socket_write(int32_t sockfd, const struct can_frame *frame)
{
  return CAN_NO_ERROR;
}

// class can_stream {
//  public:
//   using SOCKET_DESCRIPTOR = int32_t;
//   enum socket_type {
//     SOCKET_TYPE_RAW = CAN_RAW,
//     SOCKER_TYPE_ISOTP = CAN_ISOTP,
//   };
//
//   can_stream(std::string interface_name, socket_type st) : cdev{ interface_name }
//   {
//     socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
//     if (socket_fd < 0) {
//       std::cerr << "errno: " << errno << std::endl;
//       throw std::runtime_error("Failed to create socket");
//     }
//
//     memset(&ifr, 0, sizeof(ifr));
//     strncpy(ifr.ifr_name, cdev.c_str(), sizeof(ifr.ifr_name) - 1);
//     ioctl(socket_fd, SIOCGIFINDEX, &ifr);
//
//     memset(&addr, 0, sizeof(addr));
//     addr.can_family = AF_CAN;
//     addr.can_ifindex = ifr.ifr_ifindex;
//
//     if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
//       throw std::runtime_error("Failed to bind to address");
//     }
//   }
//
//  private:
//   SOCKET_DESCRIPTOR socket_fd;
//   struct ifreq ifr;
//   struct sockaddr_can addr;
//   std::string cdev = "vcan0";
// };
