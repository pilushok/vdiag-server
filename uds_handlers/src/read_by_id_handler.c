#include "uds_def.h"
#include "uds_handlers.h"
#include <stdint.h>

EXTERNC uds_error_t uds_read_by_id(struct uds_state *pstate,
                                   const uint8_t *preq, uint8_t *presp)
{
    // uint16_t data_id = (request[1] << 8) | request[2];
    //
    // response[0] = 0x62;
    // response[1] = request[1];
    // response[2] = request[2];
    //
    // switch (data_id) {
    // case 0xF190:
    //     response[3] = 0xAA;
    //     response[4] = 0xBB;
    //     return 5;
    // default:
    //     return -1;
    // }
}
