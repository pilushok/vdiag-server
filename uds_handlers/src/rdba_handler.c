// #include "uds_handlers.h"
#include "rdba_handler.h"
#include <stdint.h>
#include <stdio.h>
#include <uds_def.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define MIN_MEM_ADDR 0x0
#define MAX_MEM_ADDR 0xFFFF
#define MEM_FILENAME "/home/harg/dev/virtual_ecu/bin/state/ecu.mem"

static inline uint8_t __check_range(uint32_t uaddr, uint32_t usz)
{
    return uaddr >= MIN_MEM_ADDR && uaddr + usz <= MAX_MEM_ADDR;
}

static int32_t __read_memory(uint32_t uaddr, uint32_t usz, uint8_t *pdata)
{
    int32_t memfd   = open(MEM_FILENAME, O_RDONLY);
    int32_t ireadsz = 0;

    if (!pdata || usz == 0) {
        fprintf(stderr, "Invalid parameters: pdata=%p, usz=%u\n", (void *)pdata,
                usz);
        return -1;
    }

    if (-1 == memfd) {
        perror("memory file");
        return -1;
    }

    if (lseek(memfd, uaddr, SEEK_SET) < 0) {
        perror("memory seek");
        goto err;
    }

    ireadsz = read(memfd, pdata, usz);
    if (-1 == ireadsz) {
        perror("memory read");
        goto err;
    } else if (ireadsz != usz) {
        perror("memory read not complete");
        goto err;
    }

    return ireadsz;

err:
    if (-1 != memfd)
        close(memfd);

    return -1;
}

EXTERNC EXPORT uds_nrc_t uds_rdba_setup(struct uds_state   *puds,
                                        const can_message_t req,
                                        uds_rdba_params_t  *pparams)
{
    uint8_t  usubfnc, uaddrlen, uszlen;
    uint32_t uexplen, uaddr, usz;
    uint8_t *uaddr_raw, *usz_raw;

    // TODO: work around diag_session status
    if (!pparams || !puds) {
        return NRC_CONDITIONS_NOT_CORRECT;
    }

    if (req.usz < 2) {
        return NRC_INCORRECT_MSG_LEN_OR_FORMAT;
    }

    // if (puds->security_level < REQUIRED_SECURITY_LEVEL_FOR_MEMORY_ACCESS) {
    //     return UDS_ERROR_HANDLER_INTERNAL;
    // }

    // high and low nibbles define len of address and size
    usubfnc  = req.pdata[1];
    uaddrlen = (usubfnc >> 4) & 0x0F;
    uszlen   = usubfnc & 0x0F;
    if (uaddrlen == 0 || uaddrlen > 4 || uszlen == 0 || uszlen > 4) {
        return NRC_REQUEST_OUT_OF_RANGE;
    }

    uexplen = 2 + uaddrlen + uszlen;
    if (req.usz < uexplen) {
        return NRC_INCORRECT_MSG_LEN_OR_FORMAT;
    }

    uaddr     = 0;
    uaddr_raw = (uint8_t *)&req.pdata[2];
    for (uint8_t i = 0; i < uaddrlen; i++) {
        uaddr = (uaddr << 8) | uaddr_raw[i];
    }

    usz     = 0;
    usz_raw = (uint8_t *)&req.pdata[2 + uaddrlen];
    for (uint8_t i = 0; i < uszlen; i++) {
        usz = (usz << 8) | usz_raw[i];
    }

    if (!__check_range(uaddr, usz)) {
        return NRC_REQUEST_OUT_OF_RANGE;
    }
    pparams->uaddr = uaddr;
    pparams->usz = usz;

    return NRC_POSITIVE_RESPONSE;
}

EXTERNC EXPORT uds_rdba_result_t uds_rdba(struct uds_state       *puds,
                                          const uds_rdba_params_t params)
{
    uds_rdba_result_t res;
    memset(&res, 0, sizeof(res));
    res.pdata = (uint8_t *)calloc(MAX_DATA_CHUNK_SIZE * sizeof(uint8_t), 1);

    // TODO: work around diag_session status
    if (!puds) {
        res.rc = NRC_CONDITIONS_NOT_CORRECT;
        return res;
    }

    // Read memory (implement your hardware-specific read)
    res.usz = __read_memory(params.uaddr, params.usz, res.pdata);
    if (res.usz < 0) {
        res.rc = NRC_CONDITIONS_NOT_CORRECT;
        return res;
    }
    

    return res;
}

EXTERNC EXPORT void uds_rdba_pack(struct uds_state *puds, 
                                  uds_rdba_result_t res,
                                  struct can_message *presp)
{
    if (res.rc != NRC_POSITIVE_RESPONSE) {
        presp->pdata[0] = UDS_SID_NEGATIVE_RESPONSE;
        presp->pdata[1] = UDS_SID_READ_MEMORY_BY_ADDRESS;
        presp->pdata[2] = res.rc;
        presp->usz      = 3;
    }

    presp->pdata[0] = UDS_SID_READ_MEMORY_BY_ADDRESS | 0x40;
    memcpy(&presp[1], res.pdata, res.usz);
    presp->usz = 1 + res.usz;

    // finalize section
    free(res.pdata);
    res.pdata = NULL;
}
