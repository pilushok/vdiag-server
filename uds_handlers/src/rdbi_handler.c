#include "uds_def.h"
#include "uds_handlers.h"
#include <stdint.h>

EXTERNC EXPORT uds_error_t uds_read_by_id(struct uds_state        *puds,
                                          const struct can_message req,
                                          struct can_message      *presp)
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
    return UDS_NO_ERROR;
}
