#ifndef __UDS_H__
#define __UDS_H__

#include "can.h"
#include "uds_def.h"
#include <linux/can.h>
#include <stddef.h>
#include <stdint.h>

// handler headers
#include "tp_def.h"
#include "rdba_def.h"
#include "wrba_def.h"
#include "wrbi_def.h"
#include "rdbi_def.h"

#define STRINGIFY(X) #X

#define INIT_UDS_HANDLER_SETUP(puds, impl, handler)        \
    puds->handler.setup = (puds_##handler##_setup_t)dlsym( \
        puds->impl, STRINGIFY(uds_##handler##_setup));     \
    if (!puds->handler.setup) {                            \
        *perr = UDS_ERROR_HANDLER_INIT;                    \
        goto err;                                          \
    }

#define INIT_UDS_HANDLER_CALL(puds, impl, handler)                            \
    puds->handler.call =                                                      \
        (puds_##handler##_t)dlsym(puds->puds_impl, STRINGIFY(uds_##handler)); \
    if (!puds->handler.call) {                                                \
        *perr = UDS_ERROR_HANDLER_INIT;                                       \
        goto err;                                                             \
    }

#define INIT_UDS_HANDLER_PACK(puds, impl, handler)         \
    puds->handler.pack = (puds_##handler##_pack_t)dlsym(   \
        puds->puds_impl, STRINGIFY(uds_##handler##_pack)); \
    if (!puds->handler.pack) {                             \
        *perr = UDS_ERROR_HANDLER_INIT;                    \
        goto err;                                          \
    }

#define INIT_UDS_HANDLER(puds, impl, handler)   \
    INIT_UDS_HANDLER_SETUP(puds, impl, handler) \
    INIT_UDS_HANDLER_CALL(puds, impl, handler) \
    INIT_UDS_HANDLER_PACK(puds, impl, handler)

typedef struct uds_state {
    void *puds_impl;
#ifdef __TP_DEF_H__
    itp_t tp;
#endif // __TP_DEF_H__

#ifdef __RDBA_DEF_H__
    irdba_t rdba;
#endif

#ifdef __WRBA_DEF_H__
    iwrba_t wrba;
#endif

#ifdef __WRBI_DEF_H__
    iwrbi_t wrbi;
#endif // __WRBI_DEF_H__

#ifdef __RDBI_DEF_H__
    irdbi_t rdbi;
#endif // __RDBI_DEF_H__
      
    uint32_t client_addr;
    uint8_t  session_type;
} uds_state_t;

uds_state_t *uds_init(const char *handlers_lib, uds_error_t *perr);

uds_error_t uds_handle_msg(struct uds_state *puds, const struct can_message req,
                           struct can_message *preq);

void uds_deinit(struct uds_state **ppuds);

#endif // __UDS_H__
