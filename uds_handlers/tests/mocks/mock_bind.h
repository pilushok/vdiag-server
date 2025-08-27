#pragma once
#include <stddef.h>
#include <sys/socket.h>

enum test_bind_status {
  BIND_NO_ERROR = 0,
  BIND_FAIL = 1,
  BIND_REAL = 2
};


int __real_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int __wrap_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
