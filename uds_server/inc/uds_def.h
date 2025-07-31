#ifndef UDS_DEF_H
#define UDS_DEF_H

#include <stdint.h>

#define UDS_NEGATIVE_RESPONSE 0x7F

/**
 * @brief Negative Response Codes (NRCs) for UDS (ISO 14229-1).
 * @description These are used to indicate why a UDS request was rejected.
 */
typedef enum {
    /* --- Generic Errors --- */
    NRC_POSITIVE_RESPONSE           = 0x00, // Not an error (success)
    NRC_GENERAL_REJECT              = 0x10, // General rejection (unspecified)
    NRC_SERVICE_NOT_SUPPORTED       = 0x11, // Requested SID not implemented
    NRC_SUBFUNCTION_NOT_SUPPORTED   = 0x12, // Invalid sub-function
    NRC_INCORRECT_MSG_LEN_OR_FORMAT = 0x13, // Invalid request length/format
    NRC_RESPONSE_TOO_LONG           = 0x14, // Response exceeds buffer size

    /* --- Execution Management --- */
    NRC_BUSY_REPEAT_REQUEST        = 0x21, // ECU is busy (retry later)
    NRC_CONDITIONS_NOT_CORRECT     = 0x22, // Preconditions not met
    NRC_REQUEST_SEQUENCE_ERROR     = 0x24, // Invalid request order
    NRC_NO_RESPONSE_FROM_SUBNET    = 0x25, // Gateway couldn't reach ECU
    NRC_FAILURE_PREVENTS_EXECUTION = 0x26, // Hardware/software failure

    /* --- Parameter Issues --- */
    NRC_REQUEST_OUT_OF_RANGE = 0x31, // Parameter exceeds limits

    /* --- Security/Access --- */
    NRC_SECURITY_ACCESS_DENIED          = 0x33, // Invalid security state
    NRC_AUTHENTICATION_FAILED           = 0x34, // Failed authentication
    NRC_INVALID_KEY                     = 0x35, // Incorrect security key
    NRC_EXCEEDED_NUMBER_OF_ATTEMPTS     = 0x36, // Too many security attempts
    NRC_REQUIRED_TIME_DELAY_NOT_EXPIRED = 0x37, // Security delay timer active
    NRC_SECURE_DATA_TRANSMISSION_REQD   = 0x38, // Secure channel required
    NRC_SECURE_DATA_TRANSMISSION_NOT_ALLOWED = 0x39, // Secure comms disallowed
    NRC_SECURE_DATA_VERIFICATION_FAILED      = 0x3A, // Data verification failed

    /* --- Certificate Validation --- */
    NRC_CERT_VALIDATION_FAILED_INVALID_TIME      = 0x50,
    NRC_CERT_VALIDATION_FAILED_INVALID_SIGNATURE = 0x51,
    NRC_CERT_VALIDATION_FAILED_INVALID_CHAIN     = 0x52,
    NRC_CERT_VALIDATION_FAILED_INVALID_TYPE      = 0x53,
    NRC_CERT_VALIDATION_FAILED_INVALID_FORMAT    = 0x54,
    NRC_CERT_VALIDATION_FAILED_INVALID_CONTENT   = 0x55,
    NRC_CERT_VALIDATION_FAILED_INVALID_SCOPE     = 0x56,
    NRC_CERT_VALIDATION_FAILED_INVALID_CERT      = 0x57,
    NRC_OWNERSHIP_VERIFICATION_FAILED            = 0x58,
    NRC_CHALLENGE_CALCULATION_FAILED             = 0x59,
    NRC_SETTING_ACCESS_RIGHT_FAILED              = 0x5A,
    NRC_SESSION_KEY_CREATION_FAILED              = 0x5B,
    NRC_CONFIGURATION_DATA_USAGE_FAILED          = 0x5C,
    NRC_DEAUTHENTICATION_FAILED                  = 0x5D,

    /* --- Data Transfer --- */
    NRC_UPLOAD_DOWNLOAD_NOT_ACCEPTED = 0x70, // Transfer refused
    NRC_TRANSFER_DATA_SUSPENDED      = 0x71, // Transfer paused
    NRC_GENERAL_PROGRAMMING_FAILURE  = 0x72, // Flash programming failed
    NRC_WRONG_BLOCK_SEQUENCE_NUMBER  = 0x73, // Incorrect block order

    /* --- Session Management --- */
    NRC_RESPONSE_PENDING = 0x78, // Request processing
    NRC_SUBFUNCTION_NOT_SUPPORTED_IN_SESSION =
        0x7E, // Wrong session for sub-function
    NRC_SERVICE_NOT_SUPPORTED_IN_SESSION = 0x7F, // Wrong session for SID

    /* --- Vehicle Conditions --- */
    NRC_RPM_TOO_HIGH                   = 0x81,
    NRC_RPM_TOO_LOW                    = 0x82,
    NRC_ENGINE_IS_RUNNING              = 0x83,
    NRC_ENGINE_IS_NOT_RUNNING          = 0x84,
    NRC_ENGINE_RUN_TIME_TOO_LOW        = 0x85,
    NRC_TEMPERATURE_TOO_HIGH           = 0x86,
    NRC_TEMPERATURE_TOO_LOW            = 0x87,
    NRC_VEHICLE_SPEED_TOO_HIGH         = 0x88,
    NRC_VEHICLE_SPEED_TOO_LOW          = 0x89,
    NRC_THROTTLE_PEDAL_TOO_HIGH        = 0x8A,
    NRC_THROTTLE_PEDAL_TOO_LOW         = 0x8B,
    NRC_TRANSMISSION_NOT_IN_NEUTRAL    = 0x8C,
    NRC_TRANSMISSION_NOT_IN_GEAR       = 0x8D,
    NRC_BRAKE_SWITCH_NOT_CLOSED        = 0x8F,
    NRC_SHIFTER_LEVER_NOT_IN_PARK      = 0x90,
    NRC_TORQUE_CONVERTER_CLUTCH_LOCKED = 0x91,
    NRC_VOLTAGE_TOO_HIGH               = 0x92,
    NRC_VOLTAGE_TOO_LOW                = 0x93,
    NRC_RESOURCE_TEMPORARY_UNAVAILABLE = 0x94
} uds_nrc_t;

/**
 * @brief UDS Service Identifiers (SIDs) - ISO 14229-1
 * @note Request SIDs are odd (0x11, 0x27, etc.), response SIDs are even
 * (request SID + 0x40)
 */
typedef enum {
    /* --- Diagnostic and Control Services --- */
    UDS_SID_DIAGNOSTIC_SESSION_CONTROL = 0x10, // Not in table, but fundamental
    UDS_SID_ECU_RESET                  = 0x11, // Hard/soft reset
    UDS_SID_READ_DATA_BY_IDENTIFIER    = 0x22, // Read ECU data (VIN, sensors)
    UDS_SID_READ_MEMORY_BY_ADDRESS     = 0x23, // Read physical memory
    UDS_SID_READ_SCALING_DATA          = 0x24, // Scale data identifiers
    UDS_SID_DYNAMICALLY_DEFINE_DATA    = 0x2C, // Not in table, but common
    UDS_SID_WRITE_DATA_BY_IDENTIFIER   = 0x2E, // Not in table, but common
    UDS_SID_INPUT_OUTPUT_CONTROL       = 0x2F, // Control ECU I/O
    UDS_SID_ROUTINE_CONTROL       = 0x31, // Start/stop routines (self-tests)
    UDS_SID_REQUEST_DOWNLOAD      = 0x34, // Initiate software download
    UDS_SID_REQUEST_UPLOAD        = 0x35, // Initiate software upload
    UDS_SID_TRANSFER_DATA         = 0x36, // Data transfer (for download/upload)
    UDS_SID_REQUEST_TRANSFER_EXIT = 0x37, // End transfer
    UDS_SID_WRITE_MEMORY_BY_ADDRESS = 0x3D, // Not in table, but common
    UDS_SID_TESTER_PRESENT          = 0x3E, // Heartbeat to maintain session
    UDS_SID_CONTROL_DTC_SETTINGS    = 0x85, // Not in table, but common

    /* --- Security Services --- */
    UDS_SID_SECURITY_ACCESS = 0x27, // Authentication (basic)
    UDS_SID_SECURITY_ACCESS_ADVANCED =
        0x29,                             // PKI-based auth (table lists 0x28?)
    UDS_SID_COMMUNICATION_CONTROL = 0x28, // Enable/disable messaging

    /* --- Data Transmission Services --- */
    UDS_SID_ENABLE_DISABLE_ENCRYPTED_COMMS =
        0x84, // ISO 15764 security (table lists 0x24?)
    UDS_SID_SET_TIMING_PARAMETERS =
        0x83, // Adjust comms timing (table lists 0x30?)

    /* --- DTC Services --- */
    UDS_SID_CLEAR_DIAGNOSTIC_INFORMATION = 0x14, // Clear DTCs
    UDS_SID_READ_DTC_INFORMATION         = 0x19, // Read DTCs and snapshots

    /* --- Response SIDs (Request SID + 0x40) --- */
    UDS_SID_POSITIVE_RESPONSE = 0x40, // Base offset for positive responses
    UDS_SID_NEGATIVE_RESPONSE = 0x7F  // Negative response code
} uds_service_t;

typedef enum uds_error {
    UDS_NO_ERROR = 0,
    UDS_ERROR_MALLOC,
    UDS_ERROR_HANDLER_DIR,
    UDS_ERROR_HANDLER_INIT,
    UDS_ERROR_HANDLER_LIB_INIT,
    UDS_SERVICE_NOT_SUPPORTED,
} uds_error_t;

struct uds_state;

typedef uds_error_t (*uds_tester_present_t)(struct uds_state *puds,
                                            uint8_t *preq, uint32_t ureq_len,
                                            uint8_t  *presp,
                                            uint32_t *presp_len);
typedef uds_error_t (*uds_read_by_id_t)(uint8_t *preq, uint32_t ureq_len,
                                        uint8_t *presp, uint32_t *presp_len);
typedef uds_error_t (*uds_read_by_address_t)(uint8_t *preq, uint32_t ureq_len,
                                             uint8_t  *presp,
                                             uint32_t *presp_len);

typedef struct uds_state {
    void                 *handlers_lib;
    uds_tester_present_t  tester_present_handler;
    uds_read_by_address_t read_by_address_handler;
    uds_read_by_id_t      read_by_id_handler;
    uint32_t              client_addr;
    uint8_t               session_type;
} uds_state_t;

#endif
