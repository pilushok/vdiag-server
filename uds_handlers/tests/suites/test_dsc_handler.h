#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "dsc_def.h"
#include "uds_def.h"
#include "dsc_handler.h"
#include "minunit.h"

void dsc_setup(void)
{
    // Initialize test state if needed
}

void dsc_teardown(void)
{
    // Cleanup after each test
}

MU_TEST(test_dsc_complete_flow_extended_session)
{
    uds_state_t      puds;
    uds_dsc_params_t params;
    uds_dsc_result_t res;
    can_message_t    req, resp;
    uds_nrc_t        rc;

    uint8_t req_raw[2]   = {UDS_SID_DIAGNOSTIC_SESSION_CONTROL,
                            EXTENDED_DIAGNOSTIC_SESSION};
    uint8_t resp_raw[10] = {0};
    memcpy(req.pdata, req_raw, sizeof(req_raw));
    req.usz = sizeof(req_raw);
    memcpy(resp.pdata, resp_raw, sizeof(resp_raw));
    resp.usz = 0;

    rc = uds_dsc_setup(&puds, req, &params);
    mu_assert(rc == NRC_POSITIVE_RESPONSE, "Setup should succeed");
    mu_assert(params.uses == EXTENDED_DIAGNOSTIC_SESSION,
              "Session type should be DEFAULT_SESSION");

    res = uds_dsc(&puds, params);
    mu_assert(res.rc == NRC_POSITIVE_RESPONSE, "Service call should succeed");
    mu_assert(res.uses == EXTENDED_DIAGNOSTIC_SESSION,
              "Result should contain DEFAULT_SESSION");
    mu_assert(puds.uses == EXTENDED_DIAGNOSTIC_SESSION,
              "UDS session should be updated to DEFAULT_SESSION");

    uds_dsc_pack(&puds, res, &resp);
    mu_assert(resp.usz == 6, "Response size should be 6 bytes");

    mu_assert(resp.pdata[0] == 0x50, "Byte 0: Should be 0x50 (SID + 0x40)");
    mu_assert(resp.pdata[1] == EXTENDED_DIAGNOSTIC_SESSION,
              "Byte 1: Should be 0x03 (EXTENDED_DIAGNOSTIC_SESSION)");
}

MU_TEST(test_dsc_error_flow_unsupported_session)
{
    uds_state_t      puds;
    uds_dsc_params_t params;
    uds_dsc_result_t res;
    can_message_t    req, resp;
    uds_nrc_t        rc;
    uint8_t          req_raw[2] = {UDS_SID_DIAGNOSTIC_SESSION_CONTROL, 0x99};
    memcpy(req.pdata, req_raw, sizeof(req_raw));
    req.usz              = sizeof(req_raw);
    uint8_t resp_raw[10] = {0};
    memcpy(resp.pdata, resp_raw, sizeof(resp_raw));
    resp.usz = 0;

    rc = uds_dsc_setup(&puds, req, &params);
    mu_assert(rc == NRC_SUBFUNCTION_NOT_SUPPORTED,
              "Setup should fail with unsupported session");

    res.rc   = rc;
    res.uses = 0x01;

    uds_dsc_pack(&puds, res, &resp);
    mu_assert(resp.usz == 3, "Error response should be 3 bytes");
    mu_assert(resp.pdata[0] == 0x7F,
              "Byte 0: Should be 0x7F (negative response)");
    mu_assert(resp.pdata[1] == UDS_SID_DIAGNOSTIC_SESSION_CONTROL,
              "Byte 1: Should be 0x10 (DSC SID)");
    mu_assert(resp.pdata[2] == NRC_SUBFUNCTION_NOT_SUPPORTED,
              "Byte 2: Should be NRC code");
}

MU_TEST(test_dsc_error_flow_short_message)
{
    uds_state_t      puds;
    uds_dsc_params_t params;
    uds_dsc_result_t res;
    can_message_t    req, resp;
    uds_nrc_t        rc;
    uint8_t          req_raw[1] = {UDS_SID_DIAGNOSTIC_SESSION_CONTROL};
    memcpy(req.pdata, req_raw, sizeof(req_raw));
    req.usz              = sizeof(req_raw);
    uint8_t resp_raw[10] = {0};
    memcpy(resp.pdata, resp_raw, sizeof(resp_raw));
    resp.usz = 0;

    rc = uds_dsc_setup(&puds, req, &params);
    mu_assert(rc == NRC_INCORRECT_MSG_LEN_OR_FORMAT,
              "Setup should fail with short message");

    res.rc   = rc;
    res.uses = DEFAULT_SESSION;

    uds_dsc_pack(&puds, res, &resp);

    mu_assert(resp.usz == 3, "Error response should be 3 bytes");
    mu_assert(resp.pdata[0] == 0x7F, "Byte 0: Should be 0x7F");
    mu_assert(resp.pdata[1] == UDS_SID_DIAGNOSTIC_SESSION_CONTROL,
              "Byte 1: Should be 0x10");
    mu_assert(resp.pdata[2] == NRC_INCORRECT_MSG_LEN_OR_FORMAT,
              "Byte 2: Should be length error code");
}

MU_TEST_SUITE(dsc_suite)
{
    MU_SUITE_CONFIGURE(&dsc_setup, &dsc_teardown);
    MU_RUN_TEST(test_dsc_complete_flow_extended_session);
    MU_RUN_TEST(test_dsc_error_flow_unsupported_session);
    MU_RUN_TEST(test_dsc_error_flow_short_message);
}
