#include "uds_def.h"
#include "uds_handlers.h"
#include <stdint.h>

EXTERNC EXPORT uds_error_t
uds_diag_session_control(uds_state_t *puds, 
                         const struct can_message req,
                         struct can_message *presp)
{
    uint8_t session_type = req.pdata[1];
    puds->session_type   = session_type;

    presp->pdata[0] = 0x50;
    presp->pdata[1] = session_type;
    return 2;
}
