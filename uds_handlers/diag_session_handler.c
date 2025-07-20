#include "uds.h"
#include <stdint.h>

static int handle_diagnostic_session_control(const uint8_t *request,
                                             uint8_t *response,
                                             struct uds_state *puds) 
{
    uint8_t session_type = request[1];
    puds->session_type = session_type;

    response[0] = 0x50;
    response[1] = session_type;
    return 2;
}
