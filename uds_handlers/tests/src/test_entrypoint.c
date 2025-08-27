#include "minunit.h"

// test suites
#include "mocks/mock_socket.h"
#include "mocks/mock_bind.h"
#include "mocks/mock_calloc.h"
#include "mocks/mock_if_nametoindex.h"
#include "mocks/mock_fcntl.h"

enum test_socket_status         G_SOCKET_STATUS;
int                             G_MOCK_SOCKFD;
enum test_bind_status           G_BIND_STATUS;
enum test_calloc_status         G_CALLOC_STATUS;
enum test_if_nametoindex_status G_IF_NAMETOINDEX_STATUS;
int                             G_MOCK_IFINDEX;
enum test_fcntl_status          G_FCNTL_STATUS;
//
#include "suites/test_tp_handler.h"
int main()
{
    MU_RUN_SUITE(tp_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
