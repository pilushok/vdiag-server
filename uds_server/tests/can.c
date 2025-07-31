#include <stddef.h>
#include <stdlib.h>
#include "uds_def.h"
#include "can.h"
#include "mocks/mock_socket.h"
#include "mocks/mock_bind.h"
#include "mocks/mock_calloc.h"
#include "mocks/mock_fcntl.h"
#include "mocks/mock_if_nametoindex.h"
#include "minunit.h"

enum test_socket_status         G_SOCKET_STATUS;
int                             G_MOCK_SOCKFD;
enum test_bind_status           G_BIND_STATUS;
enum test_calloc_status         G_CALLOC_STATUS;
enum test_if_nametoindex_status G_IF_NAMETOINDEX_STATUS;
int                             G_MOCK_IFINDEX;
enum test_fcntl_status          G_FCNTL_STATUS;

void can_isotp_setup(void)
{
    /* Nothing */
}

void can_isotp_teardown(void)
{
    /* Nothing */
}

// --- Tests ---
void test_can_socket_open_success()
{
    enum can_error err    = CAN_NO_ERROR;
    const char    *ifname = "vcan0";
    uint32_t       tx_id = 0x123, rx_id = 0x456;

    // Set mock behaviors (all succeed)
    G_SOCKET_STATUS = SOCKET_SPEC;
    G_MOCK_SOCKFD   = 0;
    G_BIND_STATUS   = BIND_NO_ERROR;

    struct socket_state *sock = can_socket_open(ifname, tx_id, rx_id, &err);
    mu_assert(sock != NULL, "TEST: socket creation failed");
    mu_assert(err == CAN_NO_ERROR, "TEST: unexpected can error on open");
    mu_assert(sock->sockfd == G_MOCK_SOCKFD, "TEST: Socket FD is set");
    mu_assert(sock->addr.can_ifindex == G_MOCK_IFINDEX,
              "TEST: Interface index is set");

    G_SOCKET_STATUS = SOCKET_NO_ERROR;
    G_BIND_STATUS   = BIND_REAL;

    free(sock);
}

void test_can_socket_open_malloc_fail()
{
    // enum can_error err = CAN_NO_ERROR;
    // G_CALLOC_STATUS           = CALLOC_FAIL;
    // struct socket_state *sock = can_socket_open("vcan0", 0x123, 0x456, &err);
    // mu_assert("Socket creation fails on malloc", sock == NULL);
    // mu_assert("Error is CAN_ERROR_MALLOC", err == CAN_ERROR_MALLOC);
    // G_CALLOC_STATUS = CALLOC_NO_ERROR;
}

void test_can_socket_open_socket_fail()
{
    // enum can_error err = CAN_NO_ERROR;
    // mock_socket_ret    = -1; // Force socket() failure
    //
    // struct socket_state *sock = can_socket_open("vcan0", 0x123, 0x456, &err);
    // mu_assert("Socket creation fails on socket()", sock == NULL);
    // mu_assert("Error is CAN_ERROR_SOCKET_INIT", err ==
    // CAN_ERROR_SOCKET_INIT);
}

void test_can_socket_open_bind_fail()
{
    // enum can_error err = CAN_NO_ERROR;
    // mock_socket_ret    = 0;  // Allow socket() to succeed
    // mock_bind_ret      = -1; // Force bind() failure
    //
    // struct socket_state *sock = can_socket_open("vcan0", 0x123, 0x456, &err);
    // mu_assert("Socket creation fails on bind()", sock == NULL);
    // mu_assert("Error is CAN_ERROR_BIND", err == CAN_ERROR_BIND);
}

void test_can_socket_open_nonblock_fail()
{
    // enum can_error err    = CAN_NO_ERROR;
    // mock_socket_ret       = 0;
    // mock_bind_ret         = 0;
    // mock_set_nonblock_ret = -1; // Force set_nonblocking() failure
    //
    // struct socket_state *sock = can_socket_open("vcan0", 0x123, 0x456, &err);
    // mu_assert("Socket creation fails on nonblock", sock == NULL);
    // Note: set_nonblocking() doesn't set `perr` in original code!
}

MU_TEST_SUITE(can_isotp_suite)
{
    MU_SUITE_CONFIGURE(&can_isotp_setup, &can_isotp_teardown);

    MU_RUN_TEST(test_can_socket_open_success);
    // MU_RUN_TEST(test_can_socket_open_malloc_fail);
    // MU_RUN_TEST(test_can_socket_open_socket_fail);
    // MU_RUN_TEST(test_can_socket_open_bind_fail);
    // MU_RUN_TEST(test_can_socket_open_nonblock_fail);
}

int main()
{
    MU_RUN_SUITE(can_isotp_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
