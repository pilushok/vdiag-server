#include "minunit.h"

// test suites
#include "suites/test_can.h"

int main()
{
    MU_RUN_SUITE(can_isotp_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
