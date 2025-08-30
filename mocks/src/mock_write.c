#include "mock_write.h"
#include <errno.h>

enum test_write_status G_WRITE_STATUS = WRITE_NO_ERROR;
ssize_t G_MOCK_WRITE_RESULT = 0;

ssize_t __real_write(int fd, const void *buf, size_t count);

ssize_t __wrap_write(int fd, const void *buf, size_t count)
{
    (void)fd;
    (void)buf;
    
    switch (G_WRITE_STATUS) {
        case WRITE_NO_ERROR:
            G_MOCK_WRITE_RESULT = count;
            return count;
            
        case WRITE_ERROR:
            errno = EIO;
            G_MOCK_WRITE_RESULT = -1;
            return -1;
            
        case WRITE_PARTIAL:
            G_MOCK_WRITE_RESULT = count / 2;
            return count / 2;
            
        default:
            return __real_write(fd, buf, count);
    }
}
