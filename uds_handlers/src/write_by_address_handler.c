#include "uds_handlers.h"
#include <stdint.h>
#include <stdio.h>
#include <uds_def.h>
#include <unistd.h>
#include <fcntl.h>

#define MIN_MEM_ADDR 0x0
#define MAX_MEM_ADDR 0xFFFF
#define MEM_FILENAME "/home/harg/dev/virtual_ecu/bin/state/ecu.mem"

static uint8_t is_valid_memory_range(uint32_t uaddr, uint32_t usz)
{
    return uaddr >= MIN_MEM_ADDR && uaddr + usz <= MAX_MEM_ADDR;
}

static int32_t write_memory(uint32_t uaddr, uint32_t usz, const uint8_t *pdata)
{
    int32_t memfd = open(MEM_FILENAME, O_RDWR, NULL);
    int32_t uwritesz = 0;

    if (!pdata || usz == 0) {
        fprintf(stderr, "Invalid parameters: pdata=%p, usz=%u\n", (const void *)pdata,
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

    uwritesz = write(memfd, pdata, usz);
    if (-1 == uwritesz) {
        perror("memory write");
        goto err;
    } else if (uwritesz != usz) {
        perror("memory write not complete");
        goto err;
    }

    return uwritesz;

err:
    if (-1 != memfd)
        close(memfd);

    return -1;
}

EXTERNC uds_error_t uds_write_data_by_address(struct uds_state *puds,
                                              const uint8_t    *preq,
                                              const uint32_t    ureq_sz,
                                              uint8_t *presp, 
                                              uint32_t *presp_sz)
{
    if (!presp || !presp_sz || !preq || !puds)
        return UDS_ERROR_INVALID_PARAM;

    if (ureq_sz < 2) {
        presp[0] = UDS_SID_NEGATIVE_RESPONSE;
        presp[1] = 0x3D; // WriteMemoryByAddress SID
        presp[2] = NRC_INCORRECT_MSG_LEN_OR_FORMAT;
        *presp_sz = 3;
        return UDS_ERROR_HANDLER_INTERNAL;
    }

    // if (puds->security_level < REQUIRED_SECURITY_LEVEL_FOR_MEMORY_WRITE) {
    //     presp[0] = UDS_SID_NEGATIVE_RESPONSE;
    //     presp[1] = 0x3D;
    //     presp[2] = NRC_SECURITY_ACCESS_DENIED;
    //     *presp_sz = 3;
    //     return UDS_ERROR_HANDLER_INTERNAL;
    // }

    uint8_t uaddrlen_szlen = preq[1];
    uint8_t uaddrlen = (uaddrlen_szlen >> 4) & 0x0F;
    uint8_t uszlen = uaddrlen_szlen & 0x0F;

    if (uaddrlen == 0 || uaddrlen > 4 || uszlen == 0 || uszlen > 4) {
        presp[0] = UDS_SID_NEGATIVE_RESPONSE;
        presp[1] = 0x3D;
        presp[2] = NRC_REQUEST_OUT_OF_RANGE;
        *presp_sz = 3;
        return UDS_ERROR_HANDLER_INTERNAL;
    }

    uint32_t uexp_len = 2 + uaddrlen + uszlen;
    if (ureq_sz < uexp_len) {
        presp[0] = UDS_SID_NEGATIVE_RESPONSE;
        presp[1] = 0x3D;
        presp[2] = NRC_INCORRECT_MSG_LEN_OR_FORMAT;
        *presp_sz = 3;
        printf("ureq_sz < uexp_len\n");
        return UDS_ERROR_HANDLER_INTERNAL;
    }

    uint32_t uaddr = 0;
    const uint8_t *uaddr_bytes = &preq[2];
    for (uint8_t i = 0; i < uaddrlen; i++) {
        uaddr = (uaddr << 8) | uaddr_bytes[i];
    }
    printf("uaddr: %u\n", uaddr);

    uint32_t usz = 0;
    const uint8_t *usz_bytes = &preq[2 + uaddrlen];
    for (uint8_t i = 0; i < uszlen; i++) {
        usz = (usz << 8) | usz_bytes[i];
    }
    printf("usz: %u\n", usz);

    if (usz == 0) {
        presp[0] = UDS_SID_NEGATIVE_RESPONSE;
        presp[1] = 0x3D;
        presp[2] = NRC_REQUEST_OUT_OF_RANGE;
        *presp_sz = 3;
        return UDS_ERROR_HANDLER_INTERNAL;
    }

    uint32_t data_offset = 2 + uaddrlen + uszlen;
    printf("data_offset: %d\n", data_offset);
    if (ureq_sz < data_offset + usz) {
        presp[0] = UDS_SID_NEGATIVE_RESPONSE;
        presp[1] = 0x3D;
        presp[2] = NRC_INCORRECT_MSG_LEN_OR_FORMAT;
        *presp_sz = 3;
        printf("ureq_sz < data_offset + usz\n");
        return UDS_ERROR_HANDLER_INTERNAL;
    }

    if (!is_valid_memory_range(uaddr, usz)) {
        presp[0] = UDS_SID_NEGATIVE_RESPONSE;
        presp[1] = 0x3D;
        presp[2] = NRC_REQUEST_OUT_OF_RANGE;
        *presp_sz = 3;
        return UDS_ERROR_HANDLER_INTERNAL;
    }

    printf("preq[data_offset]: %d\n", preq[data_offset]);
    if (write_memory(uaddr, usz, &preq[data_offset]) < 0) {
        presp[0] = UDS_SID_NEGATIVE_RESPONSE;
        presp[1] = 0x3D;
        presp[2] = NRC_CONDITIONS_NOT_CORRECT;
        *presp_sz = 3;
        return UDS_ERROR_HANDLER_INTERNAL;
    }

    // Positive response echoes the request format
    presp[0] = 0x3D | 0x40; // Positive response SID
    presp[1] = uaddrlen_szlen;
    
    // Copy address bytes
    for (uint8_t i = 0; i < uaddrlen; i++) {
        presp[2 + i] = uaddr_bytes[i];
    }
    
    // Copy size bytes
    for (uint8_t i = 0; i < uszlen; i++) {
        presp[2 + uaddrlen + i] = usz_bytes[i];
    }
     
    *presp_sz = 2 + uaddrlen + uszlen;
    printf("debug presp_sz: %d\n", *presp_sz); 
    return UDS_NO_ERROR;
}
