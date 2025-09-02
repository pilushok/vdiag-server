#include <stdint.h>
#include <stdio.h>
#include <uds_def.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "wrbi_handler.h"
#include "did_map.h"
#include "utils.h"

EXTERNC EXPORT uds_nrc_t uds_wrbi_setup(struct uds_state   *puds,
                                        const can_message_t req,
                                        uds_wrbi_params_t  *pparams)
{
    uint16_t       did;
    did_mapping_t *did_map;
    init_mapping(*puds->pecucfg);
    pparams->pdata = (uint8_t *)calloc(4096 * sizeof(uint8_t), 1);

    if (!pparams || !puds) {
        return NRC_GENERAL_REJECT;
    }

    if (req.usz < 3) {
        return NRC_INCORRECT_MSG_LEN_OR_FORMAT;
    }

    // Check security access level if needed
    // if (puds->security_level < REQUIRED_SECURITY_LEVEL_FOR_WRITE) {
    //     return NRC_SECURITY_ACCESS_DENIED;
    // }
    if (puds->uses != EXTENDED_DIAGNOSTIC_SESSION) {
        return NRC_SERVICE_NOT_SUPPORTED_IN_SESSION;
    }

    did = (req.pdata[1] << 8) | req.pdata[2];
    if (did > MAX_DID) {
        return NRC_REQUEST_OUT_OF_RANGE;
    }
    did_map = &DATA_IDENTIFIER_MAP[did];

    if (!did_map->valid) {
        return NRC_REQUEST_OUT_OF_RANGE;
    }

    // Check security access level for this specific DID
    // if (puds->security_level < did_map->access_level) {
    //     return NRC_SECURITY_ACCESS_DENIED;
    // }

    // Check if the memory range is valid
    if (!check_memrange(puds->pecucfg->memory.start_addr,
                        puds->pecucfg->memory.end_addr, pparams->uaddr,
                        pparams->usz)) {
        return NRC_REQUEST_OUT_OF_RANGE;
    }

    pparams->udid  = did;
    pparams->uaddr = did_map->memory_address;
    pparams->usz   = did_map->data_size;
    memcpy(pparams->pdata, &req.pdata[3], pparams->usz);
    return NRC_POSITIVE_RESPONSE;
}

EXTERNC EXPORT uds_wrbi_result_t uds_wrbi(struct uds_state       *puds,
                                          const uds_wrbi_params_t params)
{
    uds_wrbi_result_t res;

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

    res.rc   = NRC_POSITIVE_RESPONSE;
    res.udid = params.udid;

    // finalize params
    return res;
}

EXTERNC EXPORT void uds_wrbi_pack(struct uds_state *puds, uds_wrbi_result_t res,
                                  struct can_message *presp)
{
    if (res.rc != NRC_POSITIVE_RESPONSE) {
        presp->pdata[0] = UDS_SID_NEGATIVE_RESPONSE;
        presp->pdata[1] = UDS_SID_WRITE_DATA_BY_IDENTIFIER;
        presp->pdata[2] = res.rc;
        presp->usz      = 3;
        return;
    }

    presp->pdata[0] = UDS_SID_WRITE_DATA_BY_IDENTIFIER | 0x40;
    presp->pdata[1] = (res.udid >> 8) & 0xFF;
    presp->pdata[2] = res.udid & 0xFF;
    presp->usz      = 3;

    // finalize
    // free params.pdata
}
