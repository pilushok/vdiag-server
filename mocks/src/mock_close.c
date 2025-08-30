#include "mock_close.h"
#include <errno.h>
#include <unistd.h>

enum test_close_status G_CLOSE_STATUS = CLOSE_NO_ERROR;

int __real_close(int fd);

int __wrap_close(int fd)
{
    switch (G_CLOSE_STATUS) {
        case CLOSE_NO_ERROR:
            return __real_close(fd);
            
        case CLOSE_ERROR:
            errno = EIO;
            return -1;
            
        case CLOSE_INVALID_FD:
            errno = EBADF;
            return -1;
            
        default:
            return __real_close(fd);
    }
}
