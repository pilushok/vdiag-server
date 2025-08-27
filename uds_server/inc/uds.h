#ifndef __UDS_H__
#define __UDS_H__

#include "can.h"
#include "uds_def.h"
#include <linux/can.h>
#include <stddef.h>
#include <stdint.h>

#include "rdba_def.h"

#define LOAD_UDS_HANDLER_SETUP(pstate, service_name, puds_impl)    \
    pstate->service_name##_handle = (uds_##service_name##_t)dlsym( \
        puds_impl->handlers_lib, "uds_" #service_name);            \
    if (!pstate->service_name##_handler) {                         \
        *perr = UDS_ERROR_HANDLER_INIT;                            \
        goto err;                                                  \
    }

// -----------------------------------------------------------
// typedef uds_error_t (*uds_tester_present_t)(struct uds_state *puds,
//                                             const uint8_t    *preq,
//                                             const uint32_t    ureq_len,
//                                             uint8_t          *presp,
//                                             uint32_t         *presp_len);
// typedef uds_error_t (*uds_read_by_id_t)(const uint8_t *preq,
//                                         const uint32_t ureq_len, uint8_t *presp,
//                                         uint32_t *presp_len);
// typedef uds_error_t (*uds_write_by_address_t)(struct uds_state *puds,
//                                               const uint8_t    *preq,
//                                               const uint32_t    ureq_sz,
//                                               uint8_t          *presp,
//                                               uint32_t         *presp_sz);

typedef struct uds_state {
    void *puds_impl;
    // uds_tester_present_t   tester_present_handler;
    irdba_t rdba;
    // uds_read_by_id_t       read_by_id_handler;
    // uds_write_by_address_t write_by_address_handler;
    uint32_t client_addr;
    uint8_t  session_type;
} uds_state_t;

uds_state_t *uds_init(const char *handlers_lib, uds_error_t *perr);

uds_error_t uds_handle_msg(struct uds_state *puds, const struct can_message req,
                           struct can_message *preq);

void uds_deinit(struct uds_state **ppuds);

#endif // __UDS_H__
