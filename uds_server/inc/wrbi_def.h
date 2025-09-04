#ifndef __WRBI_DEF_H__
#define __WRBI_DEF_H__

#include "can.h"
#include "uds_def.h"
#include <stdlib.h>

typedef struct uds_state uds_state_t;

typedef struct __wrbi_params {
    uint16_t udid;
    uint32_t uaddr;
    uint32_t usz;
    uint8_t *pdata;
} uds_wrbi_params_t;

typedef struct {
    uds_nrc_t rc;
    uint16_t  udid;
} uds_wrbi_result_t;

typedef uds_nrc_t (*puds_wrbi_setup_t)(uds_state_t             *puds,
                                       const struct can_message req,
                                       uds_wrbi_params_t       *pparams);

typedef uds_wrbi_result_t (*puds_wrbi_t)(uds_state_t            *puds,
                                         const uds_wrbi_params_t params);

typedef void (*puds_wrbi_pack_t)(uds_state_t *puds, const uds_wrbi_result_t res,
                                 struct can_message *presp);

static inline void
handle_wrbi_service(uds_state_t *puds, const can_message_t req,
                    can_message_t *presp, puds_wrbi_setup_t psetup,
                    puds_wrbi_t pcall, puds_wrbi_pack_t ppack)
{
    uds_wrbi_params_t wrbi_param;
    uds_wrbi_result_t wrbi_res;

    wrbi_res.rc = psetup(puds, req, &wrbi_param);
    if (wrbi_res.rc == NRC_POSITIVE_RESPONSE) {
        wrbi_res = pcall(puds, wrbi_param);
    }
    ppack(puds, wrbi_res, presp);

    // Special cleanup for WRBI
    if (wrbi_param.pdata) {
        free((void *)wrbi_param.pdata);
    }
}

typedef struct __interface_wrbi {
    puds_wrbi_setup_t setup;
    puds_wrbi_t       call;
    puds_wrbi_pack_t  pack;
} iwrbi_t;

#endif // __WRBI_DEF_H__
