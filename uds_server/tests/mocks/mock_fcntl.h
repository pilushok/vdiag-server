#include <stddef.h>
#include <fcntl.h>

enum fcntl_test_status {
  FCNTL_NO_ERROR = 0,
  FCNTL_FAIL = 1
} G_FCNTL_STATUS = FCNTL_NO_ERROR;

int __real_fcntl(int fd, int cmd, ...);

int __wrap_fcntl(int fd, int cmd, ...);
