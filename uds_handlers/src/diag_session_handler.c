#include "uds_def.h"
#include "uds_handlers.h"
#include <stdint.h>

EXTERNC uds_error_t uds_diag_session_control(struct uds_state *puds,
                                             const uint8_t *request,
                                             uint8_t *response) {
  uint8_t session_type = request[1];
  puds->session_type = session_type;

  response[0] = 0x50;
  response[1] = session_type;
  return 2;
}
