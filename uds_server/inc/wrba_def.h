#ifndef __WRBA_DEF_H__
#define __WRBA_DEF_H__

#include "can.h"
#include "uds_def.h"

typedef struct uds_state uds_state_t;

typedef struct __wrba_params {
    uint32_t uaddr;
    uint32_t usz;
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

typedef void (*puds_wrba_pack_t)(uds_state_t             *puds,
                                 const uds_wrba_result_t res,
                                 struct can_message     *presp);

typedef struct __interface_wrba {
  puds_wrba_setup_t setup;
  puds_wrba_t call;
  puds_wrba_pack_t pack;
} iwrba_t;

#endif // __WRBA_DEF_H__
