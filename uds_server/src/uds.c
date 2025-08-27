#include "uds.h"
#include "can.h"
#include "uds_def.h"
#include <dirent.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct uds_state *uds_init(const char *puds_impl, uds_error_t *perr)
{
    uds_state_t *puds = (uds_state_t *)calloc(1, sizeof(uds_state_t));
    if (puds == NULL) {
        perror("uds_state malloc");
        *perr = UDS_ERROR_MALLOC;
        goto err;
    }

    puds->puds_impl = dlopen(puds_impl, RTLD_LAZY);
    if (!puds->puds_impl) {
        perror("dlopen handlers lib");
        *perr = UDS_ERROR_HANDLER_LIB_INIT;
        goto err;
    }
    // puds->tester_present_handler =
    //     (uds_tester_present_t)dlsym(puds->puds_impl,
    //     "uds_tester_present");
    // if (!puds->tester_present_handler) {
    //     *perr = UDS_ERROR_HANDLER_INIT;
    //     goto err;
    // }
    // tp
    puds->tp.setup = (puds_tp_setup_t)dlsym(puds->puds_impl, "uds_tp_setup");
    if (!puds->tp.setup) {
        *perr = UDS_ERROR_HANDLER_INIT;
        goto err;
    }

    puds->tp.call = (puds_tp_t)dlsym(puds->puds_impl, "uds_tp");
    if (!puds->tp.call) {
        *perr = UDS_ERROR_HANDLER_INIT;
        goto err;
    }

    puds->tp.pack = (puds_tp_pack_t)dlsym(puds->puds_impl, "uds_tp_pack");
    if (!puds->tp.pack) {
        *perr = UDS_ERROR_HANDLER_INIT;
        goto err;
    }

    // rdba
    puds->rdba.setup =
        (puds_rdba_setup_t)dlsym(puds->puds_impl, "uds_rdba_setup");
    if (!puds->rdba.setup) {
        *perr = UDS_ERROR_HANDLER_INIT;
        goto err;
    }

    puds->rdba.call = (puds_rdba_t)dlsym(puds->puds_impl, "uds_rdba");
    if (!puds->rdba.call) {
        *perr = UDS_ERROR_HANDLER_INIT;
        goto err;
    }

    puds->rdba.pack = (puds_rdba_pack_t)dlsym(puds->puds_impl, "uds_rdba_pack");
    if (!puds->rdba.pack) {
        *perr = UDS_ERROR_HANDLER_INIT;
        goto err;
    }

    // wrba
    puds->wrba.setup =
        (puds_wrba_setup_t)dlsym(puds->puds_impl, "uds_wrba_setup");
    if (!puds->wrba.setup) {
        *perr = UDS_ERROR_HANDLER_INIT;
        goto err;
    }

    puds->wrba.call = (puds_wrba_t)dlsym(puds->puds_impl, "uds_wrba");
    if (!puds->wrba.call) {
        *perr = UDS_ERROR_HANDLER_INIT;
        goto err;
    }

    puds->wrba.pack = (puds_wrba_pack_t)dlsym(puds->puds_impl, "uds_wrba_pack");
    if (!puds->wrba.pack) {
        *perr = UDS_ERROR_HANDLER_INIT;
        goto err;
    }
    // puds->write_by_address_handler = (uds_write_by_address_t)dlsym(
    //     puds->puds_impl, "uds_write_data_by_address");
    // if (!puds->write_by_address_handler) {
    //     *perr = UDS_ERROR_HANDLER_INIT;
    //     goto err;
    // }

    return puds;

err:
    printf("dlsym errno: %s\n", dlerror());
    fflush(stdout);

    if (puds)
        free(puds);

    if (puds->puds_impl)
        free(puds->puds_impl);

    return NULL;
}

uds_error_t uds_handle_msg(struct uds_state *puds, const struct can_message req,
                           struct can_message *presp)
{
    uds_nrc_t         rc;
    uds_rdba_result_t rdba_res;
    uds_rdba_params_t rdba_param;

    uds_tp_result_t tp_res;
    uds_tp_params_t tp_param;
    
    uds_wrba_result_t wrba_res;
    uds_wrba_params_t wrba_param;

    if (req.usz < 1 || !presp || !puds) {
        return UDS_ERROR_INVALID_PARAM;
    }

    uint8_t service_id = req.pdata[0];
    printf("handling message service id %u\n", service_id);

    switch (service_id) {
    case UDS_SID_DIAGNOSTIC_SESSION_CONTROL:
        break;
        // return handle_diagnostic_session_control(request, response, context);

    case UDS_SID_READ_DATA_BY_IDENTIFIER:
        break;
        // return handle_read_data_by_id(request, response, context);

    case UDS_SID_READ_MEMORY_BY_ADDRESS:
        rdba_res.rc = puds->rdba.setup(puds, req, &rdba_param);

        if (rdba_res.rc == NRC_POSITIVE_RESPONSE) {
            rdba_res = puds->rdba.call(puds, rdba_param);
        }
        puds->rdba.pack(puds, rdba_res, presp);
        return UDS_NO_ERROR;
        break;

    case UDS_SID_TESTER_PRESENT:
        tp_res.rc = puds->tp.setup(puds, req, &tp_param);

        if (tp_res.rc == NRC_POSITIVE_RESPONSE) {
            tp_res = puds->tp.call(puds, tp_param);
        }
        puds->tp.pack(puds, tp_res, presp);
        return UDS_NO_ERROR;
        break;

    case UDS_SID_WRITE_MEMORY_BY_ADDRESS:
        wrba_res.rc = puds->wrba.setup(puds, req, &wrba_param);

        if (wrba_res.rc == NRC_POSITIVE_RESPONSE) {
            wrba_res = puds->wrba.call(puds, wrba_param);
        }
        puds->wrba.pack(puds, wrba_res, presp);
        return UDS_NO_ERROR;
        break;

    default:
        presp->pdata[0] = UDS_NEGATIVE_RESPONSE;
        presp->pdata[1] = service_id;
        presp->pdata[2] = NRC_SERVICE_NOT_SUPPORTED;
        return 3;
    }

    return UDS_SERVICE_NOT_SUPPORTED;
}

void uds_deinit(struct uds_state **ppuds)
{
    if (!ppuds || !*ppuds) {
        printf("warning: deinit of null uds_state suppressed\n");
        return;
    }

    if ((*ppuds)->puds_impl)
        dlclose((*ppuds)->puds_impl);
}
