#include <stddef.h>
#include <stdlib.h>
#include "uds_def.h"
#include "can.h"
#include "mock_socket.h"
#include "mock_bind.h"
#include "mock_calloc.h"
#include "mock_fcntl.h"
#include "mock_if_nametoindex.h"
#include "minunit.h"

extern enum test_socket_status         G_SOCKET_STATUS;
extern int                             G_MOCK_SOCKFD;
extern enum test_bind_status           G_BIND_STATUS;
extern enum test_calloc_status         G_CALLOC_STATUS;
extern enum test_if_nametoindex_status G_IF_NAMETOINDEX_STATUS;
extern int                             G_MOCK_IFINDEX;
extern enum test_fcntl_status          G_FCNTL_STATUS;

void can_isotp_setup(void)
{}

void can_isotp_teardown(void)
{}

MU_TEST(test_can_socket_open_success)
{
    enum can_error err    = CAN_NO_ERROR;
    const char    *ifname = "vcan0";
    uint32_t       tx_id = 0x123, rx_id = 0x456;

    // Set mock behaviors (all succeed)
    G_SOCKET_STATUS = SOCKET_SPEC;
    G_MOCK_SOCKFD   = 0;
    G_BIND_STATUS   = BIND_NO_ERROR;

    struct socket_state *sock = can_socket_open(ifname, tx_id, rx_id, &err);
    mu_assert(sock == NULL, "TEST: socket creation failed");
    mu_assert(err == CAN_NO_ERROR, "TEST: unexpected can error on open");
    mu_assert(sock->sockfd == G_MOCK_SOCKFD, "TEST: Socket FD is set");
    mu_assert(sock->addr.can_ifindex == G_MOCK_IFINDEX,
              "TEST: Interface index is set");

    G_SOCKET_STATUS = SOCKET_NO_ERROR;
    G_BIND_STATUS   = BIND_REAL;

    free(sock);
}
// TESTS
// MU_TEST(test_can_socket_open_malloc_fail);

MU_TEST_SUITE(can_isotp_suite)
{
    MU_SUITE_CONFIGURE(&can_isotp_setup, &can_isotp_teardown);
    MU_RUN_TEST(test_can_socket_open_success);
    /* Add other tests here when enabled */
}
