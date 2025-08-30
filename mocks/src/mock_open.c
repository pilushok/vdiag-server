#include "mock_open.h"
#include <errno.h>
#include <fcntl.h>

enum test_open_status G_OPEN_STATUS = OPEN_NO_ERROR;
int G_MOCK_FD = 123;

int __real_open(const char *pathname, int flags, mode_t mode);

int __wrap_open(const char *pathname, int flags, mode_t mode)
{
    (void)mode;
    
    switch (G_OPEN_STATUS) {
        case OPEN_NO_ERROR:
            return __real_open(pathname, flags, mode);
            
        case OPEN_ERROR:
            errno = ENOENT;
            return -1;
            
        case OPEN_ACCESS_DENIED:
            errno = EACCES;
            return -1;
            
        default:
            return __real_open(pathname, flags, mode);
    }
}
