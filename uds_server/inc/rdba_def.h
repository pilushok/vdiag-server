#ifndef __RDBA_DEF_H__
#define __RDBA_DEF_H__

#include "can.h"
#include "uds_def.h"

typedef struct uds_state uds_state_t;

typedef struct __rdba_params {
    uint32_t usz;
    uint32_t uaddr;
} uds_rdba_params_t;

typedef struct {
    uds_nrc_t rc;
    uint8_t  *pdata;
    uint32_t  usz;
} uds_rdba_result_t;

typedef uds_nrc_t (*puds_rdba_setup_t)(uds_state_t             *puds,
                                       const struct can_message req,
                                       uds_rdba_params_t       *pparams);

typedef uds_rdba_result_t (*puds_rdba_t)(uds_state_t            *puds,
                                         const uds_rdba_params_t params);

typedef void (*puds_rdba_pack_t)(uds_state_t *puds, const uds_rdba_result_t res,
                                 struct can_message *presp);

static inline void
handle_rdba_service(uds_state_t *puds, const can_message_t req,
                    can_message_t *presp, puds_rdba_setup_t psetup,
                    puds_rdba_t pcall, puds_rdba_pack_t ppack)
{
    uds_rdba_params_t rdba_param;
    uds_rdba_result_t rdba_res;

    rdba_res.rc = psetup(puds, req, &rdba_param);
    if (rdba_res.rc == NRC_POSITIVE_RESPONSE) {
        rdba_res = pcall(puds, rdba_param);
    }
    ppack(puds, rdba_res, presp);
}

typedef struct __interface_rdba {
    puds_rdba_setup_t setup;
    puds_rdba_t       call;
    puds_rdba_pack_t  pack;
} irdba_t;
#endif // __RDBA_DEF_H__
