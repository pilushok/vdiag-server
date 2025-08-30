#include <stddef.h>

enum test_socket_status {
    SOCKET_NO_ERROR = 0,
    SOCKET_FAIL     = 1,
    SOCKET_SPEC     = 2
};

extern enum test_socket_status G_SOCKET_STATUS;
extern int                     G_MOCK_SOCKFD;

int __real_socket(int domain, int type, int protocol);

int __wrap_socket(int domain, int type, int protocol);
