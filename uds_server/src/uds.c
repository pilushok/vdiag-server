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
    if (req.usz < 1 || !presp || !puds) {
        return UDS_ERROR_INVALID_PARAM;
    }

    usid = req.pdata[0];
    switch (usid) {
    case UDS_SID_DIAGNOSTIC_SESSION_CONTROL:
        handle_dsc_service(puds, req, presp, puds->dsc.setup, puds->dsc.call,
                           puds->dsc.pack);
        return UDS_NO_ERROR;

    case UDS_SID_READ_DATA_BY_IDENTIFIER:
        handle_rdbi_service(puds, req, presp, puds->rdbi.setup,
                            puds->rdbi.call, puds->rdbi.pack);
        return UDS_NO_ERROR;

    case UDS_SID_READ_MEMORY_BY_ADDRESS:
        handle_rdba_service(puds, req, presp, puds->rdba.setup,
                            puds->rdba.call, puds->rdba.pack);
        return UDS_NO_ERROR;

    case UDS_SID_TESTER_PRESENT:
        handle_tp_service(puds, req, presp, puds->tp.setup, puds->tp.call,
                          puds->tp.pack);
        return UDS_NO_ERROR;

    case UDS_SID_WRITE_MEMORY_BY_ADDRESS:
        handle_wrba_service(puds, req, presp, puds->wrba.setup,
                            puds->wrba.call, puds->wrba.pack);
        return UDS_NO_ERROR;

    case UDS_SID_WRITE_DATA_BY_IDENTIFIER:
        handle_wrbi_service(puds, req, presp, puds->wrbi.setup,
                            puds->wrbi.call, puds->wrbi.pack);
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
