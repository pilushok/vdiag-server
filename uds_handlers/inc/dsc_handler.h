#ifndef __DSC_HANDLER_H__
#define __DSC_HANDLER_H__

#include <stddef.h>
#include <stdint.h>
#include "common_defs.h"
#include "uds_def.h"
#include "can.h"
#include "uds.h"
#include "dsc_def.h"


EXTERNC EXPORT uds_nrc_t uds_dsc_setup(uds_state_t             *puds,
                                       const struct can_message req,
                                       uds_dsc_params_t       *pparams);

EXTERNC EXPORT uds_dsc_result_t uds_dsc(uds_state_t            *puds,
                                        const uds_dsc_params_t params);

EXTERNC EXPORT void uds_dsc_pack(uds_state_t             *puds,
                                 const uds_dsc_result_t res,
                                 struct can_message     *presp);

#endif // __DSC_HANDLER_H__
