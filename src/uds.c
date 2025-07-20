#include "uds.h"
#include <dirent.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>


struct uds_state *uds_init(const char *handler_dirname, uds_error_t *perr) 
{ 
  *perr = UDS_SERVICE_NOT_SUPPORTED;
  return NULL;
}


uds_error_t handle_uds_request(const uint8_t *request, size_t request_len,
                       uint8_t *response, size_t max_response_len,
                       struct uds_state *puds) {
    if (request_len < 1 || !response || max_response_len < 3) {
        return -1;
    }

    uint8_t service_id = request[0];

    switch (service_id) {
    case UDS_SID_DIAGNOSTIC_SESSION_CONTROL:
        break;
        // return handle_diagnostic_session_control(request, response, context);

    case UDS_SID_READ_DATA_BY_IDENTIFIER:
        break;
        // return handle_read_data_by_id(request, response, context);

    case UDS_SID_READ_MEMORY_BY_ADDRESS:
        break;

    default:
        response[0] = UDS_NEGATIVE_RESPONSE;
        response[1] = service_id;
        response[2] = NRC_SERVICE_NOT_SUPPORTED;
        return 3;
    }
    
    return UDS_SERVICE_NOT_SUPPORTED;
}
