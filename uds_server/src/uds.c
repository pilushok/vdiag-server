#include "uds.h"
#include "uds_def.h"
#include <dirent.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct uds_state *uds_init(const char *handlers_lib, uds_error_t *perr)
{
    uds_state_t *pstate = (uds_state_t *)calloc(1, sizeof(uds_state_t));
    if (pstate == NULL) {
        perror("uds_state malloc");
        *perr = UDS_ERROR_MALLOC;
        goto err;
    }

    pstate->handlers_lib = dlopen(handlers_lib, RTLD_LAZY);
    if (!pstate->handlers_lib) {
        perror("dlopen handlers lib");
        *perr = UDS_ERROR_HANDLER_LIB_INIT;
        goto err;
    }
    pstate->tester_present_handler =
        (uds_tester_present_t)dlsym(pstate->handlers_lib, "uds_tester_present");
    if (!pstate->tester_present_handler) {
        *perr = UDS_ERROR_HANDLER_INIT;
        goto err;
    }

    pstate->read_by_address_handler = (uds_read_by_address_t)dlsym(
        pstate->handlers_lib, "uds_read_data_by_address");
    if (!pstate->read_by_address_handler) {
        *perr = UDS_ERROR_HANDLER_INIT;
        goto err;
    }

    pstate->write_by_address_handler = (uds_write_by_address_t)dlsym(
        pstate->handlers_lib, "uds_write_data_by_address");
    if (!pstate->write_by_address_handler) {
        *perr = UDS_ERROR_HANDLER_INIT;
        goto err;
    }

    return pstate;

err:
    printf("dlsym errno: %s\n", dlerror());
    fflush(stdout);

    if (pstate)
        free(pstate);

    if (pstate->handlers_lib)
        free(pstate->handlers_lib);

    return NULL;
}

uds_error_t uds_handle_msg(struct uds_state *puds, const uint8_t *request,
                           const uint32_t request_len, uint8_t *response,
                           uint32_t *presp_sz)
{
    if (request_len < 1 || !response) {
        return -1;
    }

    uint8_t service_id = request[0];
    printf("handling message service id %u\n", service_id);

    switch (service_id) {
    case UDS_SID_DIAGNOSTIC_SESSION_CONTROL:
        break;
        // return handle_diagnostic_session_control(request, response, context);

    case UDS_SID_READ_DATA_BY_IDENTIFIER:
        break;
        // return handle_read_data_by_id(request, response, context);

    case UDS_SID_READ_MEMORY_BY_ADDRESS:
        puds->read_by_address_handler(puds, request, request_len, response,
                                      presp_sz);
        break;

    case UDS_SID_TESTER_PRESENT:
        puds->tester_present_handler(puds, request, request_len, response,
                                     presp_sz);
        break;

    case UDS_SID_WRITE_MEMORY_BY_ADDRESS:
        puds->write_by_address_handler(puds, request, request_len, response,
                                       presp_sz);
        break;

    default:
        response[0] = UDS_NEGATIVE_RESPONSE;
        response[1] = service_id;
        response[2] = NRC_SERVICE_NOT_SUPPORTED;
        return 3;
    }

    return UDS_SERVICE_NOT_SUPPORTED;
}
