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
    uint16_t  udid;
    uint8_t  *pdata;
    uint32_t  usz;
} uds_rdbi_result_t;

typedef uds_nrc_t (*puds_rdbi_setup_t)(uds_state_t             *puds,
                                       const struct can_message req,
                                       uds_rdbi_params_t       *pparams);

typedef uds_rdbi_result_t (*puds_rdbi_t)(uds_state_t            *puds,
                                         const uds_rdbi_params_t params);

typedef void (*puds_rdbi_pack_t)(uds_state_t *puds, const uds_rdbi_result_t res,
                                 struct can_message *presp);

static inline void handle_rdbi_service(uds_state_t *puds, const can_message_t req,
                                can_message_t *presp, puds_rdbi_setup_t psetup,
                                puds_rdbi_t pcall, puds_rdbi_pack_t ppack)
{
    uds_rdbi_params_t rdbi_param;
    uds_rdbi_result_t rdbi_res;

    rdbi_res.rc = psetup(puds, req, &rdbi_param);
    if (rdbi_res.rc == NRC_POSITIVE_RESPONSE) {
        rdbi_res = pcall(puds, rdbi_param);
    }
    ppack(puds, rdbi_res, presp);
}

typedef struct __interface_rdbi {
    puds_rdbi_setup_t setup;
    puds_rdbi_t       call;
    puds_rdbi_pack_t  pack;
} irdbi_t;

#endif // __RDBI_DEF_H__
