#include "mock_lseek.h"
#include <errno.h>
#include <unistd.h>

// Initialize mock globals
enum test_lseek_status G_LSEEK_STATUS      = LSEEK_NO_ERROR;
off_t                  G_MOCK_LSEEK_RESULT = 0;

off_t __real_lseek(int fd, off_t offset, int whence);

off_t __wrap_lseek(int fd, off_t offset, int whence)
{
    switch (G_LSEEK_STATUS) {
    case LSEEK_NO_ERROR:
        G_MOCK_LSEEK_RESULT = offset;
        return __real_lseek(fd, offset, whence);

    case LSEEK_ERROR:
        errno               = EINVAL;
        G_MOCK_LSEEK_RESULT = -1;
        return -1;

    case LSEEK_INVALID_OFFSET:
        errno               = EOVERFLOW;
        G_MOCK_LSEEK_RESULT = -1;
        return -1;

    default:
        return __real_lseek(fd, offset, whence);
    }
}
