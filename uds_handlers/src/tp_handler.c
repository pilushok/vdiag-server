#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "uds_def.h"
#include "tp_handler.h"

EXTERNC EXPORT uds_nrc_t uds_tp_setup(uds_state_t        *puds,
                                      const can_message_t req,
                                      uds_tp_params_t    *pparams)
{
    if (!puds || !pparams) {
        return NRC_GENERAL_REJECT;
    }

    if (req.usz != 2) {
        return NRC_INCORRECT_MSG_LEN_OR_FORMAT;
    }

    if (req.pdata[1] != DEFAULT_SUBFUNCTION &&
        req.pdata[1] != SUPPRESS_POS_RESPONSE_SUBFUNCTION) {
        return NRC_SUBFUNCTION_NOT_SUPPORTED;
    }

    pparams->usubfnc = req.pdata[1];
    return NRC_POSITIVE_RESPONSE;
}

EXTERNC EXPORT uds_tp_result_t uds_tp(uds_state_t *puds, uds_tp_params_t params)
{
    // This is essentially a stub virtual ecu always replies on tp now
    uds_tp_result_t res;
    res.rc    = NRC_POSITIVE_RESPONSE;
    res.usubfnc = params.usubfnc;
    printf("res.usubfnc: %d\n", res.usubfnc);
    return res;
}

EXTERNC EXPORT void uds_tp_pack(uds_state_t *puds, const uds_tp_result_t res,
                                can_message_t *presp)
{
    if (res.rc != NRC_POSITIVE_RESPONSE) {
        presp->pdata[0] = UDS_SID_NEGATIVE_RESPONSE;
        presp->pdata[1] = UDS_SID_TESTER_PRESENT;
        presp->pdata[2] = res.rc;
        presp->usz      = 3;
        return;
    }
    
    if (res.usubfnc != SUPPRESS_POS_RESPONSE_SUBFUNCTION) {
        presp->pdata[0] = UDS_SID_TESTER_PRESENT | 0x40;
        presp->pdata[1] = DEFAULT_SUBFUNCTION;
        presp->usz = 2;
    }
}
