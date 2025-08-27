#ifndef UDS_HANDLERS_H
#define UDS_HANDLERS_H

#include "can.h"
#include <stddef.h>
#include <stdint.h>

#include "common_defs.h"
#include "uds_def.h"
#include "uds.h"

EXTERNC EXPORT uds_error_t uds_tester_present(uds_state_t             *puds,
                                              const struct can_message req,
                                              struct can_message      *presp);

EXTERNC EXPORT uds_error_t uds_diag_session_control(
    uds_state_t *puds, const struct can_message req, struct can_message *presp);

EXTERNC EXPORT uds_error_t uds_read_by_id(struct uds_state        *puds,
                                          const struct can_message req,
                                          struct can_message      *presp);
EXTERNC EXPORT uds_error_t
uds_write_data_by_address(struct uds_state *puds, const struct can_message req,
                          struct can_message *presp);

#endif
