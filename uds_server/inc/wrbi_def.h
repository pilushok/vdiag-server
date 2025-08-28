#ifndef __wrbi_DEF_H__
#define __wrbi_DEF_H__

#include "can.h"
#include "uds_def.h"

typedef struct uds_state uds_state_t;

typedef struct __wrbi_params{
    uint16_t udid;
    uint32_t uaddr;
    uint32_t usz;
    const uint8_t *pdata;
} uds_wrbi_params_t;

typedef struct {
    uds_nrc_t rc;
    uint16_t udid;
} uds_wrbi_result_t;

typedef uds_nrc_t (*puds_wrbi_setup_t)(uds_state_t             *puds,
                                       const struct can_message req,
                                       uds_wrbi_params_t       *pparams);

typedef uds_wrbi_result_t (*puds_wrbi_t)(uds_state_t            *puds,
                                         const uds_wrbi_params_t params);

typedef void (*puds_wrbi_pack_t)(uds_state_t             *puds,
                                 const uds_wrbi_result_t res,
                                 struct can_message     *presp);

typedef struct __interface_wrbi {
  puds_wrbi_setup_t setup;
  puds_wrbi_t call;
  puds_wrbi_pack_t pack;
} iwrbi_t;

#endif // __wrbi_DEF_H__
