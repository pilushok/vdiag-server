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

typedef uds_nrc_t (*puds_rdba_setup_t)(uds_state_t        *puds,
                                       const struct can_message req,
                                       uds_rdba_params_t       *pparams);

typedef uds_rdba_result_t (*puds_rdba_t)(uds_state_t       *puds,
                                         const uds_rdba_params_t params);

typedef void (*puds_rdba_pack_t)(uds_state_t       *puds,
                                 const uds_rdba_result_t res,
                                 struct can_message     *presp);

typedef struct __interface_rdba {
  puds_rdba_setup_t setup;
  puds_rdba_t call;
  puds_rdba_pack_t pack;
} irdba_t;
#endif // __RDBA_DEF_H__
