#include "uds_handlers.h"
#include <stdint.h>
#include <stdio.h>
#include <uds_def.h>
#include <unistd.h>
#include <fcntl.h>

#define MIN_MEM_ADDR 0x0
#define MAX_MEM_ADDR 0xFFFF
#define MEM_FILENAME "/home/pilushok/dev/virtual_ecu/bin/state/ecu.mem"

static uint8_t is_valid_memory_range(uint32_t uaddr, uint32_t usz)
{
    return uaddr >= MIN_MEM_ADDR && uaddr + usz <= MAX_MEM_ADDR;
}

static uint8_t read_memory(uint32_t uaddr, uint32_t usz, uint8_t *pdata,
                           uint32_t ubufsz)
{
    int32_t  memfd   = open(MEM_FILENAME, O_RDONLY);
    uint32_t ureadsz = 0;
    if (-1 == memfd) {
        perror("memory file");
        return 0;
    }

    if (lseek(memfd, uaddr, SEEK_SET)) {
        perror("memory seek");
        return 0;
    }

    ureadsz = read(memfd, pdata, usz);
    if (-1 == ureadsz) {
        perror("memory read");
        return 0;
    } else if (ureadsz != usz) {
        perror("memory read not complete");
        return 0;
    }
    ubufsz = ureadsz;
    close(memfd);
    return 1;
}

EXTERNC uds_error_t uds_read_data_by_address(struct uds_state *puds,
                                             const uint8_t    *preq,
                                             const uint32_t    ureq_sz,
                                             uint8_t *presp, uint32_t *presp_sz)
{
    // TODO: work around diag_session status
    if (!presp || !presp_sz || !preq || !puds)
        return UDS_ERROR_INVALID_PARAM;

    if (ureq_sz < 2) {
        presp[0]  = UDS_SID_NEGATIVE_RESPONSE;
        presp[1]  = UDS_SID_READ_MEMORY_BY_ADDRESS;
        presp[2]  = NRC_INCORRECT_MSG_LEN_OR_FORMAT;
        *presp_sz = 3;
        return UDS_ERROR_HANDLER_INTERNAL;
    }

    uint8_t uaddrlen_szlen = preq[1];
    uint8_t uaddrlen       = (uaddrlen_szlen >> 4) & 0x0F;
    uint8_t uszlen         = uaddrlen_szlen & 0x0F;

    if (uaddrlen == 0 || uszlen == 0) {
        presp[0]  = UDS_SID_NEGATIVE_RESPONSE;
        presp[1]  = UDS_SID_READ_MEMORY_BY_ADDRESS;
        presp[2]  = NRC_INCORRECT_MSG_LEN_OR_FORMAT;
        *presp_sz = 3;
        return UDS_ERROR_HANDLER_INTERNAL;
    }

    uint32_t uexp_len = 2 + uaddrlen + uszlen;
    if (ureq_sz < uexp_len) {
        presp[0]  = UDS_SID_NEGATIVE_RESPONSE;
        presp[1]  = UDS_SID_READ_MEMORY_BY_ADDRESS;
        presp[2]  = NRC_INCORRECT_MSG_LEN_OR_FORMAT;
        *presp_sz = 3;
        return UDS_ERROR_HANDLER_INTERNAL;
    }

    uint32_t       uaddr       = 0;
    const uint8_t *uaddr_bytes = &preq[2];
    for (uint8_t i = 0; i < uaddrlen; i++) {
        uaddr = (uaddr << 8) | uaddr_bytes[i];
    }

    uint32_t       usz       = 0;
    const uint8_t *usz_bytes = &preq[2 + uaddrlen];
    for (uint8_t i = 0; i < uszlen; i++) {
        usz = (usz << 8) | usz_bytes[i];
    }

    // Validate address range (implement your specific checks)
    if (!is_valid_memory_range(uaddr, usz)) {
        presp[0]  = UDS_SID_NEGATIVE_RESPONSE;
        presp[1]  = UDS_SID_READ_MEMORY_BY_ADDRESS;
        presp[2]  = NRC_REQUEST_OUT_OF_RANGE;
        *presp_sz = 3;
        return UDS_ERROR_HANDLER_INTERNAL;
    }

    // Read memory (implement your hardware-specific read)
    if (!read_memory(uaddr, usz, &presp[2], usz)) {
        presp[0]  = UDS_SID_NEGATIVE_RESPONSE;
        presp[1]  = UDS_SID_READ_MEMORY_BY_ADDRESS;
        presp[2]  = NRC_REQUEST_OUT_OF_RANGE;
        *presp_sz = 3;
        return UDS_ERROR_HANDLER_INTERNAL;
    }

    // Build positive presp
    presp[0]  = UDS_SID_READ_MEMORY_BY_ADDRESS | 0x40;
    *presp_sz = 1 + usz;
    return UDS_NO_ERROR;
}
