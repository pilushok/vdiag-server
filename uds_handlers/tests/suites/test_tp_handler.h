#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "uds_def.h"
#include "tp_handler.h"
#include "minunit.h"

void tp_setup(void)
{}

void tp_teardown(void)
{
    // Cleanup after each test
}

MU_TEST(test_tp_setup_success_default_subfunction)
{
    uds_state_t     puds;
    uds_tp_params_t params;
    can_message_t   req;
    uds_nrc_t       rc;
    uint8_t         req_raw[2] = {UDS_SID_TESTER_PRESENT, DEFAULT_SUBFUNCTION};

    memcpy(req.pdata, req_raw, sizeof(req_raw));
    req.usz = sizeof(req_raw);

    rc = uds_tp_setup(&puds, req, &params);

    mu_assert(rc == NRC_POSITIVE_RESPONSE,
              "Setup should succeed with default subfunction");
    mu_assert(params.usubfnc == DEFAULT_SUBFUNCTION,
              "Subfunction should be correctly parsed");
}

MU_TEST(test_tp_setup_null_parameters)
{
    uds_tp_params_t params;
    can_message_t   req;
    uds_nrc_t       rc;
    uint8_t         req_raw[2] = {UDS_SID_TESTER_PRESENT, DEFAULT_SUBFUNCTION};

    memcpy(req.pdata, req_raw, sizeof(req_raw));
    req.usz = sizeof(req_raw);

    // Test null puds
    rc = uds_tp_setup(NULL, req, &params);
    mu_assert(rc == NRC_GENERAL_REJECT, "Should reject null puds");

    // Test null params
    uds_state_t puds;
    rc = uds_tp_setup(&puds, req, NULL);
    mu_assert(rc == NRC_GENERAL_REJECT, "Should reject null params");
}

MU_TEST(test_tp_setup_incorrect_length)
{
    uds_state_t     puds;
    uds_tp_params_t params;
    can_message_t   req;
    uds_nrc_t       rc;
    uint8_t         short_data[1] = {UDS_SID_TESTER_PRESENT};
    uint8_t long_data[3] = {UDS_SID_TESTER_PRESENT, DEFAULT_SUBFUNCTION, 0x00};

    memcpy(req.pdata, short_data, sizeof(short_data));
    req.usz = sizeof(short_data);
    rc      = uds_tp_setup(&puds, req, &params);
    mu_assert(rc == NRC_INCORRECT_MSG_LEN_OR_FORMAT,
              "Should reject short message");

    memcpy(req.pdata, long_data, sizeof(long_data));
    req.usz = sizeof(long_data);
    rc      = uds_tp_setup(&puds, req, &params);
    mu_assert(rc == NRC_INCORRECT_MSG_LEN_OR_FORMAT,
              "Should reject long message");
}

MU_TEST(test_tp_setup_unsupported_subfunction)
{
    uds_state_t     puds;
    uds_tp_params_t params;
    can_message_t   req;
    uds_nrc_t       rc;
    uint8_t         req_raw[2] = {UDS_SID_TESTER_PRESENT, 0x99};

    memcpy(req.pdata, req_raw, sizeof(req_raw));
    req.usz = sizeof(req_raw);
    rc      = uds_tp_setup(&puds, req, &params);
    mu_assert(rc == NRC_SUBFUNCTION_NOT_SUPPORTED,
              "Should reject unsupported subfunction");
}

MU_TEST(test_tp_integration_full_flow)
{
    uds_state_t     puds;
    uds_tp_params_t params;
    uds_tp_result_t res;
    can_message_t   req, resp;
    uds_nrc_t       rc;
    uint8_t         req_raw[2] = {UDS_SID_TESTER_PRESENT, DEFAULT_SUBFUNCTION};
    uint8_t         resp_raw[10] = {0};
    memcpy(req.pdata, req_raw, sizeof(req_raw));
    req.usz = sizeof(req_raw);
    memcpy(resp.pdata, resp_raw, sizeof(resp_raw));
    resp.usz = 0;

    // setup
    rc = uds_tp_setup(&puds, req, &params);
    mu_assert(rc == NRC_POSITIVE_RESPONSE, "Setup should succeed");
    mu_assert(params.usubfnc == DEFAULT_SUBFUNCTION,
              "Subfunction should be correctly parsed");

    // call
    res = uds_tp(&puds, params);
    mu_assert(res.rc == NRC_POSITIVE_RESPONSE, "Execute should succeed");
    mu_assert(res.usubfnc == DEFAULT_SUBFUNCTION,
              "Subfunction should be preserved");

    // pack
    uds_tp_pack(&puds, res, &resp);
    mu_assert(resp.usz == 2, "Response should be suppressed (2 bytes)");
    mu_assert(resp.pdata[0] == 0x7E, "First byte should be x7E");
    mu_assert(resp.pdata[1] == 0x00, "Second byte should be x00");
}

MU_TEST(test_tp_integration_suppress_response_flow)
{
    uds_state_t     puds;
    uds_tp_params_t params;
    uds_tp_result_t res;
    can_message_t   req, resp;
    uds_nrc_t       rc;
    uint8_t         req_raw[2]   = {UDS_SID_TESTER_PRESENT,
                                    SUPPRESS_POS_RESPONSE_SUBFUNCTION};
    uint8_t         resp_raw[10] = {0};
    memcpy(req.pdata, req_raw, sizeof(req_raw));
    req.usz = sizeof(req_raw);
    memcpy(resp.pdata, resp_raw, sizeof(resp_raw));
    resp.usz = 0;

    // setup
    rc = uds_tp_setup(&puds, req, &params);
    mu_assert(rc == NRC_POSITIVE_RESPONSE, "Setup should succeed");
    mu_assert(params.usubfnc == SUPPRESS_POS_RESPONSE_SUBFUNCTION,
              "Subfunction should be correctly parsed");

    // call
    res = uds_tp(&puds, params);
    mu_assert(res.rc == NRC_POSITIVE_RESPONSE, "Execute should succeed");
    mu_assert(res.usubfnc == SUPPRESS_POS_RESPONSE_SUBFUNCTION,
              "Subfunction should be preserved");

    // pack
    uds_tp_pack(&puds, res, &resp);
    mu_assert(resp.usz == 0, "Response should be suppressed (0 bytes)");
    mu_assert(resp.pdata[0] == 0, "Response buffer should remain unchanged");
}

MU_TEST(test_tp_integration_error_flow)
{
    uds_state_t     puds;
    uds_tp_params_t params;
    uds_tp_result_t res;
    uds_nrc_t       rc;
    can_message_t   req, resp;
    uint8_t         req_raw[2]   = {UDS_SID_TESTER_PRESENT, 0x90};
    uint8_t         resp_raw[10] = {0};
    memcpy(req.pdata, req_raw, sizeof(req_raw));
    req.usz = sizeof(req_raw);
    memcpy(resp.pdata, resp_raw, sizeof(resp_raw));
    resp.usz = 0;

    // setup
    rc = uds_tp_setup(&puds, req, &params);
    mu_assert(rc == NRC_SUBFUNCTION_NOT_SUPPORTED,
              "Setup should fail with unsupported subfunction");

    // imagery call
    res.rc      = rc;
    res.usubfnc = 0x99; // Keep the invalid subfunction

    // pack
    uds_tp_pack(&puds, res, &resp);
    mu_assert(resp.usz == 3, "Error response should be 3 bytes");
    mu_assert(resp.pdata[0] == UDS_SID_NEGATIVE_RESPONSE,
              "First byte should be negative response");
    mu_assert(resp.pdata[1] == UDS_SID_TESTER_PRESENT,
              "Second byte should be service ID");
    mu_assert(resp.pdata[2] == NRC_SUBFUNCTION_NOT_SUPPORTED,
              "Third byte should be error code");
}

MU_TEST(test_tp_integration_short_message_flow)
{
    uds_state_t     puds;
    uds_tp_params_t params;
    uds_tp_result_t res;
    uds_nrc_t       rc;
    can_message_t   req, resp;
    uint8_t         req_raw[1]   = {UDS_SID_TESTER_PRESENT};
    uint8_t         resp_raw[10] = {0};
    memcpy(req.pdata, req_raw, sizeof(req_raw));
    req.usz = sizeof(req_raw);
    memcpy(resp.pdata, resp_raw, sizeof(resp_raw));
    resp.usz = 0;

    // setup
    rc = uds_tp_setup(&puds, req, &params);
    mu_assert(rc == NRC_INCORRECT_MSG_LEN_OR_FORMAT,
              "Setup should fail with short message");
    // imagery call
    res.rc      = rc;
    res.usubfnc = 0x00; // Keep the invalid subfunction

    // pack
    uds_tp_pack(&puds, res, &resp);
    mu_assert(resp.usz == 3, "Error response should be 3 bytes");
    mu_assert(resp.pdata[0] == UDS_SID_NEGATIVE_RESPONSE,
              "First byte should be negative response");
    mu_assert(resp.pdata[1] == UDS_SID_TESTER_PRESENT,
              "Second byte should be service ID");
    mu_assert(resp.pdata[2] == NRC_INCORRECT_MSG_LEN_OR_FORMAT,
              "Third byte should be error code");
}

MU_TEST_SUITE(tp_suite)
{
    MU_SUITE_CONFIGURE(&tp_setup, &tp_teardown);

    // Setup tests
    MU_RUN_TEST(test_tp_setup_success_default_subfunction);
    MU_RUN_TEST(test_tp_setup_null_parameters);
    MU_RUN_TEST(test_tp_setup_incorrect_length);

    // flow tests
    MU_RUN_TEST(test_tp_integration_full_flow);
    MU_RUN_TEST(test_tp_integration_suppress_response_flow);
    MU_RUN_TEST(test_tp_integration_error_flow);
    MU_RUN_TEST(test_tp_integration_short_message_flow);
}
