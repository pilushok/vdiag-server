#include "mock_read.h"
#include <errno.h>
#include <unistd.h>

enum test_read_status G_READ_STATUS      = READ_NO_ERROR;
ssize_t               G_MOCK_READ_RESULT = 0;

ssize_t __real_read(int fd, void *buf, size_t count);

ssize_t __wrap_read(int fd, void *buf, size_t count)
{
    switch (G_READ_STATUS) {
    case READ_NO_ERROR:
        G_MOCK_READ_RESULT = __real_read(fd, buf, count);
        return G_MOCK_READ_RESULT;

    case READ_PARTIAL:
        G_MOCK_READ_RESULT = __real_read(fd, buf, count / 2);
        return G_MOCK_READ_RESULT;

    case READ_ERROR:
        errno              = EIO;
        G_MOCK_READ_RESULT = -1;
        return -1;

    case READ_EOF:
        G_MOCK_READ_RESULT = 0;
        return 0;

    default:
        return __real_read(fd, buf, count);
    }
}
