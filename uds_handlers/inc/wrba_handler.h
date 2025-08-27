#ifndef __WRBA_HANDLER_H__
#define __WRBA_HANDLER_H__

#include <stddef.h>
#include <stdint.h>

#include "common_defs.h"
#include "uds_def.h"
#include "can.h"
#include "uds.h"
#include "wrba_def.h"

EXTERNC EXPORT uds_nrc_t uds_wrba_setup(uds_state_t        *puds,
                                        const can_message_t req,
                                        uds_wrba_params_t  *pparams);

EXTERNC EXPORT uds_wrba_result_t uds_wrba(uds_state_t            *puds,
                                          const uds_wrba_params_t params);

EXTERNC EXPORT void uds_wrba_pack(uds_state_t            *puds,
                                  const uds_wrba_result_t res,
                                  can_message_t          *presp);

#endif // __WRBA_HANDLER_H__
