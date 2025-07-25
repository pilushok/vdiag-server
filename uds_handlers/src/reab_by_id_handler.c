#include <stdint.h>
#include "uds.h"

static int handle_read_data_by_id(const uint8_t *request, uint8_t *response,
                                  struct uds_state *ctx) {
    uint16_t data_id = (request[1] << 8) | request[2];

    response[0] = 0x62;
    response[1] = request[1];
    response[2] = request[2];

    switch (data_id) {
    case 0xF190:
        response[3] = 0xAA;
        response[4] = 0xBB;
        return 5;
    default:
        return -1;
    }
}
