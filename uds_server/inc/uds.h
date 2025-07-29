#ifndef UDS_HANDLERS_H
#define UDS_HANDLERS_H

#include "uds_def.h"
#include <stddef.h>
#include <stdint.h>

struct uds_state *uds_init(const char *handlers_lib, uds_error_t *perr);

int uds_handle_msg(const uint8_t *request, size_t request_len,
                   uint8_t *response, size_t max_response_len,
                   struct uds_state *puds);

#endif
