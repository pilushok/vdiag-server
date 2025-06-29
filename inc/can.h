#include <cerrno>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/uio.h>

#include "linux/can.h"
#include "linux/can/raw.h"
#include "net/if.h"
#include "string.h"
#include "sys/socket.h"

class can_stream {
 public:
  using SOCKET_DESCRIPTOR = int32_t;
  enum socket_type {
    SOCKET_TYPE_RAW = CAN_RAW,
    SOCKER_TYPE_ISOTP = CAN_ISOTP,
  };

  can_stream(std::string interface_name, socket_type st) : cdev{ interface_name }
  {
    socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socket_fd < 0) {
      std::cerr << "errno: " << errno << std::endl;
      throw std::runtime_error("Failed to create socket");
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, cdev.c_str(), sizeof(ifr.ifr_name) - 1);
    ioctl(socket_fd, SIOCGIFINDEX, &ifr);

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      throw std::runtime_error("Failed to bind to address");
    }
  }

 private:
  SOCKET_DESCRIPTOR socket_fd;
  struct ifreq ifr;
  struct sockaddr_can addr;
  std::string cdev = "vcan0";
};
