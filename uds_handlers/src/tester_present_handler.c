#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "uds_def.h"
#include "uds_handlers.h"

EXTERNC EXPORT uds_error_t uds_tester_present(uds_state_t *puds,
                                              const struct can_message req,
                                              struct can_message *presp)
{
    uint8_t tmp[] = {0x7E, 0x00};
    printf("Calling uds tester present handler\n");
    memcpy(presp, tmp, sizeof(tmp));
    // *presp_len = 2;
    return UDS_NO_ERROR;
}
