#ifndef UDS_HANDLERS_H
#define UDS_HANDLERS_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint32_t client_addr;
  uint8_t session_type;
} UDSContext;

int uds_handle_msg(const uint8_t* request, size_t request_len, uint8_t* response,
                       size_t max_response_len, UDSContext* context);

# define UDS_SVC_DIAG_SESSION_CONTROL 0x10
# define UDS_SVC_ECU_RESET            0x11
# define UDS_SVC_READ_DATA_BY_ID      0x22
# define UDS_SVC_WRITE_DATA_BY_ID     0x2E
# define UDS_SVC_SECURITY_ACCESS      0x27

#endif
