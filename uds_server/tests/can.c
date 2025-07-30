#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <net/if.h>
#include "can.h" // Header declaring can_socket_open()
#include "minunit.h"

// --- Mocking System Calls ---
static int mock_socket_fd        = 123; // Fake socket FD
static int mock_ifindex          = 42;  // Fake interface index
static int mock_socket_ret       = -1;  // Control socket() failure
static int mock_bind_ret         = -1;  // Control bind() failure
static int mock_set_nonblock_ret = -1;  // Control set_nonblocking() failure

extern void *(*mock_calloc)(size_t, size_t);
void *(*mock_calloc)(size_t, size_t) = calloc;

void test_setup(void)
{}

void test_teardown(void)
{
    /* Nothing */
}
// Mock fcntl
int __wrap_fcntl(int fd, int cmd, ...)
{
    (void)fd;
    if (cmd == F_GETFL)
        return 0;
    if (cmd == F_SETFL)
        return 0;
    return -1;
}

uint8_t g_calloc_fail = 0;
// Mock calloc

void *__real_calloc(size_t nmemb, size_t size);

void *__wrap_calloc(size_t nmemb, size_t size)
{
    printf("Mock calloc(%zu, %zu)\n", nmemb, size);

    if (g_calloc_fail) {
        // Force failure by returning NULL (or customize behavior)
        return NULL;
    } else {
        return __real_calloc(nmemb, size);
    }

    // Or delegate to real calloc if needed:
    // return __real_calloc(nmemb, size);
}

// Override socket()
int socket(int domain, int type, int protocol)
{
    (void)domain;
    (void)type;
    (void)protocol;
    return mock_socket_ret >= 0 ? mock_socket_fd : -1;
}

// Override bind()
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    (void)sockfd;
    (void)addr;
    (void)addrlen;
    return mock_bind_ret;
}

// Override if_nametoindex()
unsigned int if_nametoindex(const char *ifname)
{
    (void)ifname;
    return mock_ifindex;
}

// Override set_nonblocking (assume it's a separate function)
int set_nonblocking(int fd)
{
    (void)fd;
    return mock_set_nonblock_ret;
}

// --- Tests ---
void test_can_socket_open_success()
{
    enum can_error err    = CAN_NO_ERROR;
    const char    *ifname = "vcan0";
    uint32_t       tx_id = 0x123, rx_id = 0x456;

    // Set mock behaviors (all succeed)
    mock_socket_ret       = 0;
    mock_bind_ret         = 0;
    mock_set_nonblock_ret = CAN_NO_ERROR;

    struct socket_state *sock = can_socket_open(ifname, tx_id, rx_id, &err);
    mu_assert("Socket creation succeeds", sock != NULL);
    mu_assert("No error set", err == CAN_NO_ERROR);
    mu_assert("Socket FD is set", sock->sockfd == mock_socket_fd);
    mu_assert("Interface index is set", sock->addr.can_ifindex == mock_ifindex);

    free(sock);
}

void test_can_socket_open_malloc_fail()
{
    enum can_error err = CAN_NO_ERROR;

    g_calloc_fail             = 1;
    struct socket_state *sock = can_socket_open("vcan0", 0x123, 0x456, &err);
    mu_assert("Socket creation fails on malloc", sock == NULL);
    mu_assert("Error is CAN_ERROR_MALLOC", err == CAN_ERROR_MALLOC);
    g_calloc_fail = 0;
}

void test_can_socket_open_socket_fail()
{
    enum can_error err = CAN_NO_ERROR;
    mock_socket_ret    = -1; // Force socket() failure

    struct socket_state *sock = can_socket_open("vcan0", 0x123, 0x456, &err);
    mu_assert("Socket creation fails on socket()", sock == NULL);
    mu_assert("Error is CAN_ERROR_SOCKET_INIT", err == CAN_ERROR_SOCKET_INIT);
}

void test_can_socket_open_bind_fail()
{
    enum can_error err = CAN_NO_ERROR;
    mock_socket_ret    = 0;  // Allow socket() to succeed
    mock_bind_ret      = -1; // Force bind() failure

    struct socket_state *sock = can_socket_open("vcan0", 0x123, 0x456, &err);
    mu_assert("Socket creation fails on bind()", sock == NULL);
    mu_assert("Error is CAN_ERROR_BIND", err == CAN_ERROR_BIND);
}

void test_can_socket_open_nonblock_fail()
{
    enum can_error err    = CAN_NO_ERROR;
    mock_socket_ret       = 0;
    mock_bind_ret         = 0;
    mock_set_nonblock_ret = -1; // Force set_nonblocking() failure

    struct socket_state *sock = can_socket_open("vcan0", 0x123, 0x456, &err);
    mu_assert("Socket creation fails on nonblock", sock == NULL);
    // Note: set_nonblocking() doesn't set `perr` in original code!
}

MU_TEST_SUITE(test_suite)
{
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_can_socket_open_success);
    MU_RUN_TEST(test_can_socket_open_malloc_fail);
    // MU_RUN_TEST(test_can_socket_open_socket_fail);
    // MU_RUN_TEST(test_can_socket_open_bind_fail);
    // MU_RUN_TEST(test_can_socket_open_nonblock_fail);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
