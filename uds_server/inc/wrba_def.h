#ifndef __WRBA_DEF_H__
#define __WRBA_DEF_H__

#include "can.h"
#include "uds_def.h"

typedef struct uds_state uds_state_t;

typedef struct __wrba_params {
    uint32_t       uaddr;
    uint32_t       usz;
    const uint8_t *pdata;
} uds_wrba_params_t;

typedef struct {
    uds_nrc_t rc;
} uds_wrba_result_t;

typedef uds_nrc_t (*puds_wrba_setup_t)(uds_state_t             *puds,
                                       const struct can_message req,
                                       uds_wrba_params_t       *pparams);

typedef uds_wrba_result_t (*puds_wrba_t)(uds_state_t            *puds,
                                         const uds_wrba_params_t params);

typedef void (*puds_wrba_pack_t)(uds_state_t *puds, const uds_wrba_result_t res,
                                 struct can_message *presp);

static inline void handle_wrba_service(uds_state_t *puds, const can_message_t req,
                                can_message_t *presp, puds_wrba_setup_t psetup,
                                puds_wrba_t pcall, puds_wrba_pack_t ppack)
{
    uds_wrba_params_t wrba_param;
    uds_wrba_result_t wrba_res;

    wrba_res.rc = psetup(puds, req, &wrba_param);

    if (wrba_res.rc == NRC_POSITIVE_RESPONSE) {
        wrba_res = pcall(puds, wrba_param);
    }
    ppack(puds, wrba_res, presp);
}

typedef struct __interface_wrba {
    puds_wrba_setup_t setup;
    puds_wrba_t       call;
    puds_wrba_pack_t  pack;
} iwrba_t;

#endif // __WRBA_DEF_H__
