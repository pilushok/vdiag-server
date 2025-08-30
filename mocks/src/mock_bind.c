#include "mock_bind.h"

enum test_bind_status G_BIND_STATUS;

int __wrap_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    switch (G_BIND_STATUS) {
    case BIND_NO_ERROR:
        return 0;
    case BIND_FAIL:
        (void)sockfd;
        (void)addr;
        (void)addrlen;
        return -1;
    case BIND_REAL:
        return __real_bind(sockfd, addr, addrlen);
    }
}

