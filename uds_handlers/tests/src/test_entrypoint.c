#include "minunit.h"

// test suites
#include "mock_socket.h"
#include "mock_bind.h"
#include "mock_calloc.h"
#include "mock_if_nametoindex.h"
#include "mock_fcntl.h"

// test suites 
#include "suites/test_tp_handler.h"
#include "suites/test_dsc_handler.h"
#include "suites/test_wrba_handler.h"

int main()
{
    MU_RUN_SUITE(tp_suite);
    MU_RUN_SUITE(dsc_suite);
    MU_RUN_SUITE(wrba_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
