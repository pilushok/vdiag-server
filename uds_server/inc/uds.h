#ifndef UDS_HANDLERS_H
#define UDS_HANDLERS_H

#include "uds_def.h"
#include <stddef.h>
#include <stdint.h>

struct uds_state *uds_init(const char *handlers_lib, uds_error_t *perr);

uds_error_t uds_handle_msg(struct uds_state *puds, const uint8_t *request,
                           const uint32_t request_len, uint8_t *response,
                           uint32_t *presp_len);

#endif
