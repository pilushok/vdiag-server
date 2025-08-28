#ifndef __DSC_DEF_H__
#define __DSC_DEF_H__

#include "can.h"
#include "uds_def.h"

// Diagnostic Session Types
#define DEFAULT_SESSION 0x01
#define PROGRAMMING_SESSION 0x02
#define EXTENDED_DIAGNOSTIC_SESSION 0x03
#define SAFETY_SYSTEM_DIAGNOSTIC_SESSION 0x04

typedef struct uds_state uds_state_t;

typedef struct __dsc_params {
    uint8_t uses;
} uds_dsc_params_t;

typedef struct {
    uds_nrc_t rc;
    uint8_t uses;
    uint16_t p2_server_max;
    uint16_t p2_star_server_max;
} uds_dsc_result_t;

typedef uds_nrc_t (*puds_dsc_setup_t)(uds_state_t *puds,
                                      const struct can_message req,
                                      uds_dsc_params_t *pparams);

typedef uds_dsc_result_t (*puds_dsc_t)(uds_state_t *puds,
                                       const uds_dsc_params_t params);

typedef void (*puds_dsc_pack_t)(uds_state_t *puds,
                                const uds_dsc_result_t res,
                                struct can_message *presp);

typedef struct __interface_dsc {
    puds_dsc_setup_t setup;
    puds_dsc_t call;
    puds_dsc_pack_t pack;
} idsc_t;

#endif // __DSC_DEF_H__
