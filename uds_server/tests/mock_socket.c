#include "mocks/mock_socket.h"

extern enum test_socket_status G_SOCKET_STATUS;
extern int                     G_MOCK_SOCKFD;

int __wrap_socket(int domain, int type, int protocol)
{
    switch (G_SOCKET_STATUS) {
    case SOCKET_NO_ERROR:
        return __real_socket(domain, type, protocol);

    case SOCKET_FAIL:
        (void)domain;
        (void)type;
        (void)protocol;
        return -1;

    case SOCKET_SPEC:
        return G_MOCK_SOCKFD;
    }
}
