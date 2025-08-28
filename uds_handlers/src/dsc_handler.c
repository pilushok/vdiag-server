#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "dsc_handler.h"
#include "uds_def.h"

#define P2_SERVER_MAX_DEFAULT 50
#define P2_SERVER_MAX_PROGRAMMING 5000
#define P2_STAR_SERVER_MAX_DEFAULT 5000

EXTERNC EXPORT uds_nrc_t uds_dsc_setup(uds_state_t             *puds,
                                       const struct can_message req,
                                       uds_dsc_params_t        *pparams)
{
    if (!pparams || !puds) {
        return NRC_GENERAL_REJECT;
    }

    if (req.usz != 2) {
        return NRC_INCORRECT_MSG_LEN_OR_FORMAT;
    }

    uint8_t uses = req.pdata[1];

    switch (uses) {
    case DEFAULT_SESSION:
    case PROGRAMMING_SESSION:
    case EXTENDED_DIAGNOSTIC_SESSION:
    case SAFETY_SYSTEM_DIAGNOSTIC_SESSION:
        break;
    default:
        return NRC_SUBFUNCTION_NOT_SUPPORTED;
    }

    pparams->uses = uses;
    return NRC_POSITIVE_RESPONSE;
}

EXTERNC EXPORT uds_dsc_result_t uds_dsc(uds_state_t           *puds,
                                        const uds_dsc_params_t params)
{
    uds_dsc_result_t res;
    memset(&res, 0, sizeof(res));

    if (!puds) {
        res.rc = NRC_CONDITIONS_NOT_CORRECT;
        return res;
    }

    puds->uses = params.uses;

    // Set session timing parameters based on session type
    switch (params.uses) {
    case DEFAULT_SESSION:
        res.p2_server_max      = P2_SERVER_MAX_DEFAULT;
        res.p2_star_server_max = P2_STAR_SERVER_MAX_DEFAULT;
        break;
    case PROGRAMMING_SESSION:
        res.p2_server_max      = P2_SERVER_MAX_PROGRAMMING;
        res.p2_star_server_max = P2_STAR_SERVER_MAX_DEFAULT;
        break;
    case EXTENDED_DIAGNOSTIC_SESSION:
    case SAFETY_SYSTEM_DIAGNOSTIC_SESSION:
        res.p2_server_max      = P2_SERVER_MAX_DEFAULT;
        res.p2_star_server_max = P2_STAR_SERVER_MAX_DEFAULT;
        break;
    }

    res.rc   = NRC_POSITIVE_RESPONSE;
    res.uses = params.uses;
    return res;
}

EXTERNC EXPORT void uds_dsc_pack(uds_state_t *puds, const uds_dsc_result_t res,
                                 struct can_message *presp)
{
    if (res.rc != NRC_POSITIVE_RESPONSE) {
        presp->pdata[0] = UDS_SID_NEGATIVE_RESPONSE;
        presp->pdata[1] = UDS_SID_DIAGNOSTIC_SESSION_CONTROL;
        presp->pdata[2] = res.rc;
        presp->usz      = 3;
        return;
    }

    // Positive response: SID + session type + timing parameters
    presp->pdata[0] = UDS_SID_DIAGNOSTIC_SESSION_CONTROL | 0x40;
    presp->pdata[1] = res.uses;
    presp->pdata[2] = (res.p2_server_max >> 8) & 0xFF;
    presp->pdata[3] = res.p2_server_max & 0xFF;
    presp->pdata[4] = (res.p2_star_server_max >> 8) & 0xFF;
    presp->pdata[5] = res.p2_star_server_max & 0xFF;
    presp->usz      = 6;
}
