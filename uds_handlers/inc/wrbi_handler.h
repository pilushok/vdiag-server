#ifndef __WRBI_HANDLER_H__
#define __WRBI_HANDLER_H__

#include <stddef.h>
#include <stdint.h>
#include "common_defs.h"
#include "uds_def.h"
#include "can.h"
#include "uds.h"
#include "wrbi_def.h"


EXTERNC EXPORT uds_nrc_t uds_wrbi_setup(uds_state_t             *puds,
                                        const struct can_message req,
                                        uds_wrbi_params_t       *pparams);

EXTERNC EXPORT uds_wrbi_result_t uds_wrbi(uds_state_t            *puds,
                                          const uds_wrbi_params_t params);

EXTERNC EXPORT void uds_wrbi_pack(uds_state_t             *puds,
                                  const uds_wrbi_result_t res,
                                  struct can_message     *presp);

#endif // __WRBI_HANDLER_H__
