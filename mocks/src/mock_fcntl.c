#include "mock_fcntl.h"

enum test_fcntl_status G_FCNTL_STATUS;

int __wrap_fcntl(int fd, int cmd, ...)
{
    switch (G_FCNTL_STATUS) {
    case FCNTL_NO_ERROR:
        return __real_fcntl(fd, cmd);
    case FCNTL_FAIL:
        (void)fd;
        if (cmd == F_GETFL)
            return 0;
        if (cmd == F_SETFL)
            return 0;
        return -1;
    }
}
