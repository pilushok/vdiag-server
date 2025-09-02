#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "uds_def.h"
#include "wrba_handler.h"
#include "minunit.h"
#include "mock_open.h"
#include "mock_lseek.h"
#include "mock_write.h"
#include "mock_close.h"

static ecu_config_t ecucfg;

void init_config_hardcoded(ecu_config_t *config)
{
    if (config == NULL)
        return;

    memset(config, 0, sizeof(ecu_config_t));

    // [can] section
    strncpy(config->can.interface, "vcan0", sizeof(config->can.interface));
    config->can.tx_id = 0x7E0;
    config->can.rx_id = 0x7E8;
    strncpy(config->can.protocol, "isotp", sizeof(config->can.protocol));

    // [uds] section
    strncpy(config->uds.cimpl_libname,
            "/home/harg/dev/virtual_ecu/bin/uds_handlers/std_handlers.so",
            sizeof(config->uds.cimpl_libname));

    // [memory] section
    strncpy(config->memory.file_path,
            "/home/harg/dev/virtual_ecu/bin/state/test.mem",
            sizeof(config->memory.file_path));
    config->memory.start_addr = 0x0000;
    config->memory.end_addr   = 0xFFFF;

    // [data_identifiers] section
    config->did_count = 4;

    // F100 = 0x1000:4:1
    strncpy(config->did_names[0], "F100", sizeof(config->did_names[0]));
    config->data_identifiers[0].address = 0x1000;
    config->data_identifiers[0].udid    = 0xF100; // Convert "F100" to 0xF100
    config->data_identifiers[0].size    = 4;
    config->data_identifiers[0].access_level = 1;

    // F101 = 0x2000:2:1
    strncpy(config->did_names[1], "F101", sizeof(config->did_names[1]));
    config->data_identifiers[1].address = 0x2000;
    config->data_identifiers[1].udid    = 0xF101; // Convert "F101" to 0xF101
    config->data_identifiers[1].size    = 2;
    config->data_identifiers[1].access_level = 1;

    // F102 = 0x3000:8:2
    strncpy(config->did_names[2], "F102", sizeof(config->did_names[2]));
    config->data_identifiers[2].address = 0x3000;
    config->data_identifiers[2].udid    = 0xF102; // Convert "F102" to 0xF102
    config->data_identifiers[2].size    = 8;
    config->data_identifiers[2].access_level = 2;

    // F103 = 0x4000:16:1
    strncpy(config->did_names[3], "F103", sizeof(config->did_names[3]));
    config->data_identifiers[3].address = 0x4000;
    config->data_identifiers[3].udid    = 0xF103; // Convert "F103" to 0xF103
    config->data_identifiers[3].size    = 16;
    config->data_identifiers[3].access_level = 1;
}

void wrba_setup(void)
{
    G_OPEN_STATUS       = OPEN_NO_ERROR;
    G_MOCK_FD           = 123;
    G_LSEEK_STATUS      = LSEEK_NO_ERROR;
    G_MOCK_LSEEK_RESULT = 0;
    G_WRITE_STATUS      = WRITE_NO_ERROR;
    G_MOCK_WRITE_RESULT = 0;
    G_CLOSE_STATUS      = CLOSE_NO_ERROR;
    init_config_hardcoded(&ecucfg);
}

void wrba_teardown(void)
{
    // Cleanup after each test
}

MU_TEST(test_wrba_complete_flow_success)
{
    uds_state_t       puds;
    uds_wrba_params_t params;
    uds_wrba_result_t res;
    can_message_t     req, resp;
    uds_nrc_t         rc;

    puds.uses           = EXTENDED_DIAGNOSTIC_SESSION;
    puds.pecucfg        = &ecucfg;
    uint8_t req_raw[10] = {
        UDS_SID_WRITE_MEMORY_BY_ADDRESS, // SID
        0x22, // Subfunction: 2-byte addr, 2-byte size
        0x10,
        0x00, // Address: 0x1000
        0x00,
        0x04, // Size: 4 bytes
        0xAA,
        0xBB,
        0xCC,
        0xDD // Data to write
    };
    memcpy(req.pdata, req_raw, sizeof(req_raw));
    req.usz = sizeof(req_raw);

    uint8_t resp_raw[10] = {0};
    memcpy(resp.pdata, resp_raw, sizeof(resp_raw));
    resp.usz = 0;

    G_OPEN_STATUS  = OPEN_SPEC;
    G_WRITE_STATUS = WRITE_NO_ERROR;
    rc             = uds_wrba_setup(&puds, req, &params);
    mu_assert(rc == NRC_POSITIVE_RESPONSE, "Setup should succeed");
    mu_assert(params.uaddr == 0x1000, "Address should be 0x1000");
    mu_assert(params.usz == 4, "Size should be 4 bytes");
    mu_assert(memcmp(params.pdata, (uint8_t[]){0xAA, 0xBB, 0xCC, 0xDD}, 4) == 0,
              "Data should match");

    res = uds_wrba(&puds, params);

    mu_assert(res.rc == NRC_POSITIVE_RESPONSE, "Service call should succeed");
    uds_wrba_pack(&puds, res, &resp);
    mu_assert(resp.usz == 1, "Response size should be 1 byte");
    mu_assert(resp.pdata[0] == 0x7D,
              "Byte 0: Should be 0x5E (WRBA SID + 0x40)");
}

MU_TEST(test_wrba_error_flow_open_failure)
{
    uds_state_t       puds;
    uds_wrba_params_t params;
    uds_wrba_result_t res;
    can_message_t     req, resp;
    uds_nrc_t         rc;
    puds.uses           = EXTENDED_DIAGNOSTIC_SESSION;
    puds.pecucfg        = &ecucfg;
    uint8_t req_raw[10] = {UDS_SID_WRITE_MEMORY_BY_ADDRESS,
                           0x22,
                           0x10,
                           0x00,
                           0x00,
                           0x04,
                           0xAA,
                           0xBB,
                           0xCC,
                           0xDD};

    uint8_t resp_raw[10] = {0};

    memcpy(req.pdata, req_raw, sizeof(req_raw));
    req.usz = sizeof(req_raw);
    memcpy(resp.pdata, resp_raw, sizeof(resp_raw));
    resp.usz = 0;

    G_OPEN_STATUS = OPEN_ERROR;
    rc            = uds_wrba_setup(&puds, req, &params);
    mu_assert(rc == NRC_POSITIVE_RESPONSE, "Setup should succeed");

    res = uds_wrba(&puds, params);
    mu_assert(res.rc == NRC_CONDITIONS_NOT_CORRECT,
              "Service call should fail with file open error");

    uds_wrba_pack(&puds, res, &resp);
    mu_assert(resp.usz == 3, "Error response should be 3 bytes");
    mu_assert(resp.pdata[0] == 0x7F,
              "Byte 0: Should be 0x7F (negative response)");
    mu_assert(resp.pdata[1] == UDS_SID_WRITE_MEMORY_BY_ADDRESS,
              "Byte 1: Should be WRBA SID");
    mu_assert(resp.pdata[2] == NRC_CONDITIONS_NOT_CORRECT,
              "Byte 2: Should be conditions error code");
}

MU_TEST(test_wrba_error_flow_wrong_session)
{
    uds_state_t       puds;
    uds_wrba_params_t params;
    uds_wrba_result_t res;
    can_message_t     req, resp;
    uds_nrc_t         rc;
    puds.uses            = DEFAULT_SESSION;
    puds.pecucfg        = &ecucfg;
    uint8_t req_raw[10]  = {UDS_SID_WRITE_MEMORY_BY_ADDRESS,
                            0x22,
                            0x10,
                            0x00,
                            0x00,
                            0x04,
                            0xAA,
                            0xBB,
                            0xCC,
                            0xDD};
    uint8_t resp_raw[10] = {0};
    memcpy(req.pdata, req_raw, sizeof(req_raw));
    req.usz = sizeof(req_raw);
    memcpy(resp.pdata, resp_raw, sizeof(resp_raw));
    resp.usz = 0;

    rc = uds_wrba_setup(&puds, req, &params);
    mu_assert(rc == NRC_SERVICE_NOT_SUPPORTED_IN_SESSION,
              "Setup should fail with wrong session");

    res.rc = rc;

    uds_wrba_pack(&puds, res, &resp);
    mu_assert(resp.usz == 3, "Error response should be 3 bytes");
    mu_assert(resp.pdata[0] == 0x7F, "Byte 0: Should be 0x7F");
    mu_assert(resp.pdata[1] == UDS_SID_WRITE_MEMORY_BY_ADDRESS,
              "Byte 1: Should be WRBA SID");
    mu_assert(resp.pdata[2] == NRC_SERVICE_NOT_SUPPORTED_IN_SESSION,
              "Byte 2: Should be session error code");
}

MU_TEST(test_wrba_error_flow_short_message)
{
    uds_state_t       puds;
    uds_wrba_params_t params;
    uds_wrba_result_t res;
    can_message_t     req, resp;
    uds_nrc_t         rc;

    puds.uses = EXTENDED_DIAGNOSTIC_SESSION;
    puds.pecucfg        = &ecucfg;
    uint8_t req_raw[6] = {
        UDS_SID_WRITE_MEMORY_BY_ADDRESS, 0x22, 0x10, 0x00, 0x00, 0x04};
    uint8_t resp_raw[10] = {0};
    memcpy(req.pdata, req_raw, sizeof(req_raw));
    req.usz = sizeof(req_raw);
    memcpy(resp.pdata, resp_raw, sizeof(resp_raw));
    resp.usz = 0;

    rc = uds_wrba_setup(&puds, req, &params);
    mu_assert(rc == NRC_INCORRECT_MSG_LEN_OR_FORMAT,
              "Setup should fail with short message");

    res.rc = rc;

    uds_wrba_pack(&puds, res, &resp);
    mu_assert(resp.usz == 3, "Error response should be 3 bytes");
    mu_assert(resp.pdata[0] == 0x7F, "Byte 0: Should be 0x7F");
    mu_assert(resp.pdata[1] == UDS_SID_WRITE_MEMORY_BY_ADDRESS,
              "Byte 1: Should be WRBA SID");
    mu_assert(resp.pdata[2] == NRC_INCORRECT_MSG_LEN_OR_FORMAT,
              "Byte 2: Should be length error code");
}

MU_TEST_SUITE(wrba_suite)
{
    MU_SUITE_CONFIGURE(&wrba_setup, &wrba_teardown);

    // Complete flow tests
    MU_RUN_TEST(test_wrba_complete_flow_success);
    MU_RUN_TEST(test_wrba_error_flow_open_failure);
    MU_RUN_TEST(test_wrba_error_flow_wrong_session);
    MU_RUN_TEST(test_wrba_error_flow_short_message);
}
