#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "uds_def.h"
#include "tp_handler.h"
#include "minunit.h"

// Mock the printf function to capture output
char printf_buffer[256];
int printf_call_count = 0;

// int mock_printf(const char *format, ...)
// {
//     printf_call_count++;
//     va_list args;
//     va_start(args, format);
//     vsnprintf(printf_buffer, sizeof(printf_buffer), format, args);
//     va_end(args);
//     return 0;
// }
//
// // Replace the real printf with our mock
// #define printf mock_printf

void tp_setup(void)
{
    // Reset mock state before each test
    printf_call_count = 0;
    memset(printf_buffer, 0, sizeof(printf_buffer));
}

void tp_teardown(void)
{
    // Cleanup after each test
}

MU_TEST(test_tp_setup_success_default_subfunction)
{
    uds_state_t puds;
    uds_tp_params_t params;
    can_message_t req;
    uint8_t request_data[2] = {UDS_SID_TESTER_PRESENT, DEFAULT_SUBFUNCTION};
    
    memcpy(req.pdata, request_data, sizeof(request_data));
    req.usz = sizeof(request_data);
    
    uds_nrc_t result = uds_tp_setup(&puds, req, &params);
    
    mu_assert(result == NRC_POSITIVE_RESPONSE, "Setup should succeed with default subfunction");
    mu_assert(params.usubfnc == DEFAULT_SUBFUNCTION, "Subfunction should be correctly parsed");
}

// MU_TEST(test_tp_setup_success_suppress_response)
// {
//     uds_state_t puds;
//     uds_tp_params_t params;
//     can_message_t req;
//     uint8_t request_data[2] = {UDS_SID_TESTER_PRESENT, SUPPRESS_POS_RESPONSE_SUBFUNCTION};
//
//     req.pdata = request_data;
//     req.usz = sizeof(request_data);
//
//     uds_nrc_t result = uds_tp_setup(&puds, req, &params);
//
//     mu_assert(result == NRC_POSITIVE_RESPONSE, "Setup should succeed with suppress response");
//     mu_assert(params.usubfnc == SUPPRESS_POS_RESPONSE_SUBFUNCTION, "Subfunction should be correctly parsed");
// }
//
// MU_TEST(test_tp_setup_null_parameters)
// {
//     can_message_t req;
//     uint8_t request_data[2] = {UDS_SID_TESTER_PRESENT, DEFAULT_SUBFUNCTION};
//
//     req.pdata = request_data;
//     req.usz = sizeof(request_data);
//
//     // Test null puds
//     uds_tp_params_t params;
//     uds_nrc_t result = uds_tp_setup(NULL, req, &params);
//     mu_assert(result == NRC_GENERAL_REJECT, "Should reject null puds");
//
//     // Test null params
//     uds_state_t puds;
//     result = uds_tp_setup(&puds, req, NULL);
//     mu_assert(result == NRC_GENERAL_REJECT, "Should reject null params");
// }
//
// MU_TEST(test_tp_setup_incorrect_length)
// {
//     uds_state_t puds;
//     uds_tp_params_t params;
//     can_message_t req;
//
//     // Test too short
//     uint8_t short_data[1] = {UDS_SID_TESTER_PRESENT};
//     req.pdata = short_data;
//     req.usz = sizeof(short_data);
//
//     uds_nrc_t result = uds_tp_setup(&puds, req, &params);
//     mu_assert(result == NRC_INCORRECT_MSG_LEN_OR_FORMAT, "Should reject short message");
//
//     // Test too long
//     uint8_t long_data[3] = {UDS_SID_TESTER_PRESENT, DEFAULT_SUBFUNCTION, 0x00};
//     req.pdata = long_data;
//     req.usz = sizeof(long_data);
//
//     result = uds_tp_setup(&puds, req, &params);
//     mu_assert(result == NRC_INCORRECT_MSG_LEN_OR_FORMAT, "Should reject long message");
// }
//
// MU_TEST(test_tp_setup_unsupported_subfunction)
// {
//     uds_state_t puds;
//     uds_tp_params_t params;
//     can_message_t req;
//     uint8_t request_data[2] = {UDS_SID_TESTER_PRESENT, 0x99}; // Invalid subfunction
//
//     req.pdata = request_data;
//     req.usz = sizeof(request_data);
//
//     uds_nrc_t result = uds_tp_setup(&puds, req, &params);
//     mu_assert(result == NRC_SUBFUNCTION_NOT_SUPPORTED, "Should reject unsupported subfunction");
// }
//
// MU_TEST(test_tp_execute_success_default_subfunction)
// {
//     uds_state_t puds;
//     uds_tp_params_t params = {.usubfnc = DEFAULT_SUBFUNCTION};
//
//     uds_tp_result_t result = uds_tp(&puds, params);
//
//     mu_assert(result.rc == NRC_POSITIVE_RESPONSE, "Execute should succeed");
//     mu_assert(result.usubfnc == DEFAULT_SUBFUNCTION, "Subfunction should be preserved");
//     mu_assert(printf_call_count == 1, "printf should be called once");
//     mu_assert(strstr(printf_buffer, "res.usubfnc:") != NULL, "printf should contain debug message");
// }
//
// MU_TEST(test_tp_execute_success_suppress_response)
// {
//     uds_state_t puds;
//     uds_tp_params_t params = {.usubfnc = SUPPRESS_POS_RESPONSE_SUBFUNCTION};
//
//     uds_tp_result_t result = uds_tp(&puds, params);
//
//     mu_assert(result.rc == NRC_POSITIVE_RESPONSE, "Execute should succeed");
//     mu_assert(result.usubfnc == SUPPRESS_POS_RESPONSE_SUBFUNCTION, "Subfunction should be preserved");
// }
//
// MU_TEST(test_tp_pack_success_default_subfunction)
// {
//     uds_state_t puds;
//     uds_tp_result_t result = {
//         .rc = NRC_POSITIVE_RESPONSE,
//         .usubfnc = DEFAULT_SUBFUNCTION
//     };
//     can_message_t resp;
//     uint8_t resp_buffer[10] = {0};
//     resp.pdata = resp_buffer;
//
//     uds_tp_pack(&puds, result, &resp);
//
//     mu_assert(resp.usz == 2, "Response should be 2 bytes");
//     mu_assert(resp.pdata[0] == (UDS_SID_TESTER_PRESENT | 0x40), 
//               "First byte should be positive response SID");
//     mu_assert(resp.pdata[1] == DEFAULT_SUBFUNCTION, 
//               "Second byte should be default subfunction");
// }
//
// MU_TEST(test_tp_pack_success_suppress_response)
// {
//     uds_state_t puds;
//     uds_tp_result_t result = {
//         .rc = NRC_POSITIVE_RESPONSE,
//         .usubfnc = SUPPRESS_POS_RESPONSE_SUBFUNCTION
//     };
//     can_message_t resp;
//     uint8_t resp_buffer[10] = {0};
//     resp.pdata = resp_buffer;
//
//     uds_tp_pack(&puds, result, &resp);
//
//     mu_assert(resp.usz == 0, "Response should be suppressed (0 bytes)");
//     mu_assert(resp.pdata[0] == 0, "Response buffer should be unchanged");
// }
//
// MU_TEST(test_tp_pack_error_response)
// {
//     uds_state_t puds;
//     uds_tp_result_t result = {
//         .rc = NRC_SUBFUNCTION_NOT_SUPPORTED,
//         .usubfnc = DEFAULT_SUBFUNCTION
//     };
//     can_message_t resp;
//     uint8_t resp_buffer[10] = {0};
//     resp.pdata = resp_buffer;
//
//     uds_tp_pack(&puds, result, &resp);
//
//     mu_assert(resp.usz == 3, "Error response should be 3 bytes");
//     mu_assert(resp.pdata[0] == UDS_SID_NEGATIVE_RESPONSE, 
//               "First byte should be negative response");
//     mu_assert(resp.pdata[1] == UDS_SID_TESTER_PRESENT, 
//               "Second byte should be service ID");
//     mu_assert(resp.pdata[2] == NRC_SUBFUNCTION_NOT_SUPPORTED, 
//               "Third byte should be error code");
// }

MU_TEST_SUITE(tp_suite)
{
    MU_SUITE_CONFIGURE(&tp_setup, &tp_teardown);
    
    // Setup tests
    MU_RUN_TEST(test_tp_setup_success_default_subfunction);
    // MU_RUN_TEST(test_tp_setup_success_suppress_response);
    // MU_RUN_TEST(test_tp_setup_null_parameters);
    // MU_RUN_TEST(test_tp_setup_incorrect_length);
    // MU_RUN_TEST(test_tp_setup_unsupported_subfunction);
    //
    // // Execute tests
    // MU_RUN_TEST(test_tp_execute_success_default_subfunction);
    // MU_RUN_TEST(test_tp_execute_success_suppress_response);
    //
    // // Pack tests
    // MU_RUN_TEST(test_tp_pack_success_default_subfunction);
    // MU_RUN_TEST(test_tp_pack_success_suppress_response);
    // MU_RUN_TEST(test_tp_pack_error_response);
}
