#ifndef __RDBI_DEF_H__
#define __RDBI_DEF_H__

#include "can.h"
#include "uds_def.h"

typedef struct uds_state uds_state_t;

typedef struct __rdbi_params {
    uint16_t udid;
    uint32_t uaddr;
    uint32_t usz;
} uds_rdbi_params_t;

typedef struct {
    uds_nrc_t rc;
    uint16_t udid;
    uint8_t *pdata;
    uint32_t usz;  
} uds_rdbi_result_t;

typedef uds_nrc_t (*puds_rdbi_setup_t)(uds_state_t             *puds,
                                       const struct can_message req,
                                       uds_rdbi_params_t       *pparams);

typedef uds_rdbi_result_t (*puds_rdbi_t)(uds_state_t            *puds,
                                         const uds_rdbi_params_t params);

typedef void (*puds_rdbi_pack_t)(uds_state_t             *puds,
                                 const uds_rdbi_result_t res,
                                 struct can_message     *presp);

typedef struct __interface_rdbi {
  puds_rdbi_setup_t setup;
  puds_rdbi_t call;
  puds_rdbi_pack_t pack;
} irdbi_t;

#endif // __RDBI_DEF_H__
