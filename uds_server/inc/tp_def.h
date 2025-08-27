#ifndef __TP_DEF_H__
#define __TP_DEF_H__

#include "stdbool.h"
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
    uint32_t usubfnc;
} uds_tp_result_t;

typedef uds_nrc_t (*puds_tp_setup_t)(uds_state_t *puds, can_message_t req,
                                     uds_tp_params_t *pparams);

typedef uds_tp_result_t (*puds_tp_t)(uds_state_t          *puds,
                                     const uds_tp_params_t params);

typedef void (*puds_tp_pack_t)(uds_state_t *puds, const uds_tp_result_t res,
                               can_message_t *presp);

typedef struct __interface_tp {
    puds_tp_setup_t setup;
    puds_tp_t       call;
    puds_tp_pack_t  pack;
} itp_t;
#endif // __TP_DEF_H__
