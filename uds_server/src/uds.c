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

#ifdef __TP_DEF_H__
    INIT_UDS_HANDLER(puds, puds_impl, tp)
#endif // __TP_DEF_H__

#ifdef __WRBA_DEF_H__
    INIT_UDS_HANDLER(puds, puds_impl, wrba)
#endif // __WRBA_DEF_H__

#ifdef __RDBA_DEF_H__
    INIT_UDS_HANDLER(puds, puds_impl, rdba)
#endif // __RDBA_DEF_H__

#ifdef __WRBI_DEF_H__
    INIT_UDS_HANDLER(puds, puds_impl, wrbi)
#endif // __WRBI_DEF_H__

#ifdef __RDBI_DEF_H__
    INIT_UDS_HANDLER(puds, puds_impl, rdbi)
#endif // __RDBI_DEF_H__

#ifdef __DSC_DEF_H__
    INIT_UDS_HANDLER(puds, puds_impl, dsc)
#endif // __DSC_DEF_H__

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
    uint8_t usid;
    uds_nrc_t         rc;
    uds_rdba_result_t rdba_res;
    uds_rdba_params_t rdba_param;

    uds_tp_result_t tp_res;
    uds_tp_params_t tp_param;

    uds_wrba_result_t wrba_res;
    uds_wrba_params_t wrba_param;

    uds_wrbi_result_t wrbi_res;
    uds_wrbi_params_t wrbi_param;

    uds_rdbi_result_t rdbi_res;
    uds_rdbi_params_t rdbi_param;

    uds_dsc_result_t dsc_res;
    uds_dsc_params_t dsc_param;

    if (req.usz < 1 || !presp || !puds) {
        return UDS_ERROR_INVALID_PARAM;
    }

    usid = req.pdata[0];
    switch (usid) {

    case UDS_SID_DIAGNOSTIC_SESSION_CONTROL:
        dsc_res.rc = puds->dsc.setup(puds, req, &dsc_param);
        if (dsc_res.rc == NRC_POSITIVE_RESPONSE) {
            dsc_res = puds->dsc.call(puds, dsc_param);
        }
        puds->dsc.pack(puds, dsc_res, presp);
        return UDS_NO_ERROR;

    case UDS_SID_READ_DATA_BY_IDENTIFIER:
        rdbi_res.rc = puds->rdbi.setup(puds, req, &rdbi_param);

        if (rdbi_res.rc == NRC_POSITIVE_RESPONSE) {
            rdbi_res = puds->rdbi.call(puds, rdbi_param);
        }
        puds->rdbi.pack(puds, rdbi_res, presp);
        return UDS_NO_ERROR;

    case UDS_SID_READ_MEMORY_BY_ADDRESS:
        rdba_res.rc = puds->rdba.setup(puds, req, &rdba_param);

        if (rdba_res.rc == NRC_POSITIVE_RESPONSE) {
            rdba_res = puds->rdba.call(puds, rdba_param);
        }
        puds->rdba.pack(puds, rdba_res, presp);
        return UDS_NO_ERROR;

    case UDS_SID_TESTER_PRESENT:
        tp_res.rc = puds->tp.setup(puds, req, &tp_param);

        if (tp_res.rc == NRC_POSITIVE_RESPONSE) {
            tp_res = puds->tp.call(puds, tp_param);
        }
        puds->tp.pack(puds, tp_res, presp);
        return UDS_NO_ERROR;

    case UDS_SID_WRITE_MEMORY_BY_ADDRESS:
        wrba_res.rc = puds->wrba.setup(puds, req, &wrba_param);

        if (wrba_res.rc == NRC_POSITIVE_RESPONSE) {
            wrba_res = puds->wrba.call(puds, wrba_param);
        }
        puds->wrba.pack(puds, wrba_res, presp);
        return UDS_NO_ERROR;

    case UDS_SID_WRITE_DATA_BY_IDENTIFIER:
        wrbi_res.rc = puds->wrbi.setup(puds, req, &wrbi_param);

        if (wrbi_res.rc == NRC_POSITIVE_RESPONSE) {
            wrbi_res = puds->wrbi.call(puds, wrbi_param);
        }
        puds->wrbi.pack(puds, wrbi_res, presp);
        free(wrbi_param.pdata);
        return UDS_NO_ERROR;

    default:
        presp->pdata[0] = UDS_NEGATIVE_RESPONSE;
        presp->pdata[1] = usid;
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
