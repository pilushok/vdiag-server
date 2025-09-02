#include <stdint.h>
#include <stdio.h>
#include <uds_def.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "wrba_handler.h"
#include "dsc_def.h"
#include "utils.h"

EXTERNC EXPORT uds_nrc_t uds_wrba_setup(struct uds_state   *puds,
                                        const can_message_t req,
                                        uds_wrba_params_t  *pparams)
{
    uint8_t  usubfnc, uaddrlen, uszlen;
    uint32_t uexplen, uaddr, usz;
    uint8_t *uaddr_raw, *usz_raw;
    uint32_t data_offset;

    if (!pparams || !puds) {
        return NRC_GENERAL_REJECT;
    }

    if (req.usz < 2) {
        return NRC_INCORRECT_MSG_LEN_OR_FORMAT;
    }

    // if (puds->security_level < REQUIRED_SECURITY_LEVEL_FOR_MEMORY_ACCESS) {
    //     return UDS_ERROR_HANDLER_INTERNAL;
    // }

    if (puds->uses != EXTENDED_DIAGNOSTIC_SESSION) {
        return NRC_SERVICE_NOT_SUPPORTED_IN_SESSION;
    }

    // high and low nibbles define len of address and size
    usubfnc  = req.pdata[1];
    uaddrlen = (usubfnc >> 4) & 0x0F;
    uszlen   = usubfnc & 0x0F;
    if (uaddrlen == 0 || uaddrlen > 4 || uszlen == 0 || uszlen > 4) {
        return NRC_REQUEST_OUT_OF_RANGE;
    }

    uexplen = 2 + uaddrlen + uszlen;
    if (req.usz < uexplen) {
        return NRC_INCORRECT_MSG_LEN_OR_FORMAT;
    }

    uaddr     = 0;
    uaddr_raw = (uint8_t *)&req.pdata[2];
    for (uint8_t i = 0; i < uaddrlen; i++) {
        uaddr = (uaddr << 8) | uaddr_raw[i];
    }

    usz     = 0;
    usz_raw = (uint8_t *)&req.pdata[2 + uaddrlen];
    for (uint8_t i = 0; i < uszlen; i++) {
        usz = (usz << 8) | usz_raw[i];
    }

    if (!check_memrange(puds->pecucfg->memory.start_addr,
                        puds->pecucfg->memory.end_addr, uaddr, usz)) {
        return NRC_REQUEST_OUT_OF_RANGE;
    }

    // Verify we have enough data for the write operation
    data_offset = 2 + uaddrlen + uszlen;
    if (req.usz < data_offset + usz) {
        return NRC_INCORRECT_MSG_LEN_OR_FORMAT;
    }

    pparams->uaddr = uaddr;
    pparams->usz   = usz;
    pparams->pdata = (uint8_t *)&req.pdata[data_offset];

    return NRC_POSITIVE_RESPONSE;
}

EXTERNC EXPORT uds_wrba_result_t uds_wrba(struct uds_state       *puds,
                                          const uds_wrba_params_t params)
{
    uds_wrba_result_t res;
    memset(&res, 0, sizeof(res));

    if (!puds) {
        res.rc = NRC_CONDITIONS_NOT_CORRECT;
        return res;
    }

    // Write memory
    if (write_memory(puds->pecucfg->memory.file_path, params.uaddr, params.usz,
                     params.pdata) < 0) {
        res.rc = NRC_CONDITIONS_NOT_CORRECT;
        return res;
    }

    res.rc = NRC_POSITIVE_RESPONSE;
    return res;
}

EXTERNC EXPORT void uds_wrba_pack(struct uds_state *puds, uds_wrba_result_t res,
                                  struct can_message *presp)
{
    if (res.rc != NRC_POSITIVE_RESPONSE) {
        presp->pdata[0] = UDS_SID_NEGATIVE_RESPONSE;
        presp->pdata[1] = UDS_SID_WRITE_MEMORY_BY_ADDRESS;
        presp->pdata[2] = res.rc;
        presp->usz      = 3;
        return;
    }

    // Positive response for write is just the service ID + 0x40
    presp->pdata[0] = UDS_SID_WRITE_MEMORY_BY_ADDRESS | 0x40;
    presp->usz      = 1;
}
