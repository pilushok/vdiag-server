#ifndef UDS_HANDLERS_H
#define UDS_HANDLERS_H

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* so export */
#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define CALLBACK
#define EXPORT __attribute__((visibility("default")))
#endif

#include "uds_def.h"

EXTERNC EXPORT uds_error_t uds_tester_present(struct uds_state *puds,
                                              const uint8_t    *preq,
                                              uint32_t preq_len, uint8_t *presp,
                                              uint32_t *presp_len);

EXTERNC EXPORT uds_error_t uds_diag_session_control(struct uds_state *puds,
                                                    const uint8_t    *request,
                                                    uint8_t          *response);

EXTERNC EXPORT uds_error_t uds_read_by_id(struct uds_state *puds,
                                          const uint8_t    *request,
                                          uint8_t          *response);

EXTERNC EXPORT uds_error_t uds_read_data_by_address(struct uds_state *puds,
                                                    const uint8_t    *preq,
                                                    const uint32_t    ureq_sz,
                                                    uint8_t          *presp,
                                                    uint32_t         *presp_sz);

EXTERNC EXPORT uds_error_t uds_write_data_by_address(struct uds_state *puds,
                                                     const uint8_t    *preq,
                                                     const uint32_t    ureq_sz,
                                                     uint8_t          *presp,
                                                     uint32_t *presp_sz);

#endif
