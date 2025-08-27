#include <stddef.h>
#include <stdint.h>

#include "common_defs.h"
#include "uds_def.h"
#include "can.h"
#include "uds.h"

#define MAX_DATA_CHUNK_SIZE 4096

EXTERNC EXPORT uds_nrc_t uds_rdba_setup(uds_state_t             *puds,
                                        const struct can_message req,
                                        uds_rdba_params_t       *pparams);

EXTERNC EXPORT uds_rdba_result_t uds_rdba(uds_state_t            *puds,
                                          const uds_rdba_params_t params);

EXTERNC EXPORT void uds_rdba_pack(uds_state_t            *puds,
                                  const uds_rdba_result_t res,
                                  struct can_message     *presp);
