#include <stdint.h>
#include <stdio.h>
#include <uds_def.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "wrbi_handler.h"
#include "did_map.h"

#define MIN_MEM_ADDR 0x0
#define MAX_MEM_ADDR 0xFFFF
#define MEM_FILENAME "/home/harg/dev/virtual_ecu/bin/state/ecu.mem"
#define MAX_DID 0xFFFF

static inline uint8_t __check_range(uint32_t uaddr, uint32_t usz)
{
    return uaddr >= MIN_MEM_ADDR && uaddr + usz <= MAX_MEM_ADDR;
}

static int32_t __write_memory(uint32_t uaddr, uint32_t usz, const uint8_t *pdata)
{
    int32_t memfd = open(MEM_FILENAME, O_WRONLY);
    int32_t iwritesz = 0;

    if (!pdata || usz == 0) {
        fprintf(stderr, "Invalid parameters: pdata=%p, usz=%u\n", (void *)pdata, usz);
        return -1;
    }

    if (memfd < 0) {
        perror("memory file");
        return -1;
    }

    if (lseek(memfd, uaddr, SEEK_SET) < 0) {
        perror("memory seek");
        goto err;
    }
    
    printf("wrbi pdata: %X %X\n", pdata[0], pdata[1]);
    printf("addr, usz: %u %u\n", uaddr, usz);
    iwritesz = write(memfd, pdata, usz);
    if (iwritesz < 0) {
        perror("memory write");
        goto err;
    } else if (iwritesz != usz) {
        perror("memory write not complete");
        goto err;
    }

    close(memfd);
    return iwritesz;

err:
    if (memfd != -1)
        close(memfd);
    return -1;
}

EXTERNC EXPORT uds_nrc_t uds_wrbi_setup(struct uds_state   *puds,
                                        const can_message_t req,
                                        uds_wrbi_params_t  *pparams)
{
    uint16_t did;
    did_mapping_t *did_map;
    init_mapping(); 
    pparams->pdata = (uint8_t *) calloc(4096 * sizeof(uint8_t), 1);

    if (!pparams || !puds) {
        return NRC_GENERAL_REJECT;
    }

    if (req.usz < 3) {
        return NRC_INCORRECT_MSG_LEN_OR_FORMAT;
    }

    // Check security access level if needed
    // if (puds->security_level < REQUIRED_SECURITY_LEVEL_FOR_WRITE) {
    //     return NRC_SECURITY_ACCESS_DENIED;
    // }

    did = (req.pdata[1] << 8) | req.pdata[2];

    if (did > MAX_DID) {
        printf("max did\n"); 
        return NRC_REQUEST_OUT_OF_RANGE;
    }
    did_map = &DATA_IDENTIFIER_MAP[did];
    
    if (!did_map->valid) {
        printf("invalid did: %d\n", did);
        return NRC_REQUEST_OUT_OF_RANGE;
    }

    // Check security access level for this specific DID
    // if (puds->security_level < did_map->access_level) {
    //     return NRC_SECURITY_ACCESS_DENIED;
    // }

    // Check if the memory range is valid
    if (!__check_range(did_map->memory_address, did_map->data_size)) {
        printf("__check_range\n");
        return NRC_REQUEST_OUT_OF_RANGE;
    }

    pparams->udid = did;
    pparams->uaddr = did_map->memory_address;
    pparams->usz = did_map->data_size;
    // pparams->pdata = (uint8_t *)&req.pdata[3]; // Data starts after SID + DID
    memcpy(pparams->pdata, &req.pdata[3], pparams->usz);
    // printf("wrbi pdata: %X %X\n", pparams->pdata[0], pparams->pdata[1]);
    // printf("addr, usz: %u %u\n", pparams->uaddr, pparams->usz);
    return NRC_POSITIVE_RESPONSE;
}

EXTERNC EXPORT uds_wrbi_result_t uds_wrbi(struct uds_state       *puds,
                                          const uds_wrbi_params_t params)
{
    uds_wrbi_result_t res;

    if (!puds) {
        res.rc = NRC_CONDITIONS_NOT_CORRECT;
        return res;
    }

    // Write memory
    if (__write_memory(params.uaddr, params.usz, params.pdata) < 0) {
        res.rc = NRC_CONDITIONS_NOT_CORRECT;
        return res;
    }

    res.rc = NRC_POSITIVE_RESPONSE;
    res.udid = params.udid;
  
    // finalize params
    return res;
}

EXTERNC EXPORT void uds_wrbi_pack(struct uds_state *puds, uds_wrbi_result_t res,
                                  struct can_message *presp)
{
    if (res.rc != NRC_POSITIVE_RESPONSE) {
        presp->pdata[0] = UDS_SID_NEGATIVE_RESPONSE;
        presp->pdata[1] = UDS_SID_WRITE_DATA_BY_IDENTIFIER;
        presp->pdata[2] = res.rc;
        presp->usz = 3;
        return;
    }

    presp->pdata[0] = UDS_SID_WRITE_DATA_BY_IDENTIFIER | 0x40;
    presp->pdata[1] = (res.udid >> 8) & 0xFF;
    presp->pdata[2] = res.udid & 0xFF;
    presp->usz = 3;

    // finalize 
  // free params.pdata
}
