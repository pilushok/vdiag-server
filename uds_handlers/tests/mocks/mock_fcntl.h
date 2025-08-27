#pragma once
#include <stddef.h>
#include <fcntl.h>

enum test_fcntl_status {
  FCNTL_NO_ERROR = 0,
  FCNTL_FAIL = 1
};

int __real_fcntl(int fd, int cmd, ...);

int __wrap_fcntl(int fd, int cmd, ...);
