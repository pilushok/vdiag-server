#ifndef __TP_HANDLER_H__
#define __TP_HANDLER_H__

#include "common_defs.h"
#include "uds_def.h"
#include "uds.h"
#include "tp_def.h"

EXTERNC EXPORT uds_nrc_t uds_tp_setup(uds_state_t        *puds,
                                      const can_message_t req,
                                      uds_tp_params_t    *pparams);

EXTERNC EXPORT uds_tp_result_t uds_tp(uds_state_t    *puds,
                                      uds_tp_params_t params);

EXTERNC EXPORT void uds_tp_pack(uds_state_t *puds, const uds_tp_result_t res,
                                can_message_t *presp);

#endif // !__TP_HANDLER_H__
