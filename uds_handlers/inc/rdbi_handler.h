#ifndef __RDBI_HANDLER_H__
#define __RDBI_HANDLER_H__

#include <stddef.h>
#include <stdint.h>
#include "common_defs.h"
#include "uds_def.h"
#include "can.h"
#include "uds.h"
#include "rdbi_def.h"

EXTERNC EXPORT uds_nrc_t uds_rdbi_setup(uds_state_t             *puds,
                                        const struct can_message req,
                                        uds_rdbi_params_t       *pparams);

EXTERNC EXPORT uds_rdbi_result_t uds_rdbi(uds_state_t            *puds,
                                          const uds_rdbi_params_t params);

EXTERNC EXPORT void uds_rdbi_pack(uds_state_t             *puds,
                                  const uds_rdbi_result_t res,
                                  struct can_message     *presp);

#endif // __RDBI_HANDLER_H__
