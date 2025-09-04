#ifndef __TP_DEF_H__
#define __TP_DEF_H__

#include "can.h"
#include "uds_def.h"

#define DEFAULT_SUBFUNCTION 0x00
#define SUPPRESS_POS_RESPONSE_SUBFUNCTION 0x80

typedef struct uds_state uds_state_t;

typedef struct __tp_params {
    uint32_t usubfnc;
} uds_tp_params_t;

typedef struct {
    uds_nrc_t rc;
    uint32_t  usubfnc;
} uds_tp_result_t;

typedef uds_nrc_t (*puds_tp_setup_t)(uds_state_t *puds, can_message_t req,
                                     uds_tp_params_t *pparams);

typedef uds_tp_result_t (*puds_tp_t)(uds_state_t          *puds,
                                     const uds_tp_params_t params);

typedef void (*puds_tp_pack_t)(uds_state_t *puds, const uds_tp_result_t res,
                               can_message_t *presp);

static inline void handle_tp_service(uds_state_t *puds, const can_message_t req,
                              can_message_t *presp, puds_tp_setup_t psetup,
                              puds_tp_t pcall, puds_tp_pack_t ppack)
{
    uds_tp_params_t tp_param;
    uds_tp_result_t tp_res;

    tp_res.rc = psetup(puds, req, &tp_param);
    if (tp_res.rc == NRC_POSITIVE_RESPONSE) {
        tp_res = pcall(puds, tp_param);
    }
    ppack(puds, tp_res, presp);
}

typedef struct __interface_tp {
    puds_tp_setup_t setup;
    puds_tp_t       call;
    puds_tp_pack_t  pack;
} itp_t;
#endif // __TP_DEF_H__
