#include <string.h>

#include "uds.h"

static int handle_diagnostic_session_control(const uint8_t* request, uint8_t* response,
                                             UDSContext* ctx)
{
  uint8_t session_type = request[1];
  ctx->session_type = session_type;

  response[0] = 0x50;
  response[1] = session_type;
  return 2;
}

static int handle_read_data_by_id(const uint8_t* request, uint8_t* response, UDSContext* ctx)
{
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

int handle_uds_request(const uint8_t* request, size_t request_len, uint8_t* response,
                       size_t max_response_len, UDSContext* context)
{
  if (request_len < 1 || !response || max_response_len < 3) {
    return -1;
  }

  uint8_t service_id = request[0];

  switch (service_id) {
  case UDS_SVC_DIAG_SESSION_CONTROL:
    return handle_diagnostic_session_control(request, response, context);

  case UDS_SVC_READ_DATA_BY_ID:
    return handle_read_data_by_id(request, response, context);

  default:
    response[0] = 0x7F;
    response[1] = service_id;
    response[2] = 0x11;
    return 3;
  }
}
