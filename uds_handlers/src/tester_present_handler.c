#include <stdio.h>
#include "uds_def.h"
#include "uds_handlers.h"

EXTERNC EXPORT uds_error_t uds_tester_present(struct uds_state *puds,
                                       const uint8_t *preq, 
                                       uint32_t preq_len,
                                       uint8_t *presp,
                                       uint32_t *presp_len) {
  printf("Calling uds tester present handler");
  return UDS_NO_ERROR;
}
