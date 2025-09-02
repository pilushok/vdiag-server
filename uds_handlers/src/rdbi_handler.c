#include <stdint.h>
#include <stdio.h>
#include <uds_def.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "rdbi_handler.h"
#include "did_map.h"

#define MIN_MEM_ADDR 0x0
#define MAX_MEM_ADDR 0xFFFF
#define MEM_FILENAME "/home/harg/dev/virtual_ecu/bin/state/ecu.mem"
#define MAX_DID 0xFFFF

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

    if (memfd < 0) {
        perror("memory file");
        return -1;
    }

    if (lseek(memfd, uaddr, SEEK_SET) < 0) {
        perror("memory seek");
        goto err;
    }

    ireadsz = read(memfd, pdata, usz);
    if (ireadsz < 0) {
        perror("memory read");
        goto err;
    } else if (ireadsz != usz) {
        perror("memory read not complete");
        goto err;
    }

    close(memfd);
    return ireadsz;

err:
    if (memfd != -1)
        close(memfd);
    return -1;
}

EXTERNC EXPORT uds_nrc_t uds_rdbi_setup(struct uds_state   *puds,
                                        const can_message_t req,
                                        uds_rdbi_params_t  *pparams)
{
    uint16_t       udid;
    did_mapping_t *did_map;
    init_mapping(*puds->pecucfg);

    if (!pparams || !puds) {
        return NRC_GENERAL_REJECT;
    }

    if (req.usz < 3) {
        return NRC_INCORRECT_MSG_LEN_OR_FORMAT;
    }

    // Check security access level if needed
    // if (puds->security_level < REQUIRED_SECURITY_LEVEL_FOR_READ) {
    //     return NRC_SECURITY_ACCESS_DENIED;
    // }

    udid = (req.pdata[1] << 8) | req.pdata[2];

    if (udid > MAX_DID) {
        return NRC_REQUEST_OUT_OF_RANGE;
    }
    did_map = &DATA_IDENTIFIER_MAP[udid];

    if (!did_map->valid) {
        return NRC_REQUEST_OUT_OF_RANGE;
    }

    // Check security access level for this specific DID
    // if (puds->security_level < did_map->access_level) {
    //     return NRC_SECURITY_ACCESS_DENIED;
    // }

    // Check if the memory range is valid
    if (!__check_range(did_map->memory_address, did_map->data_size)) {
        return NRC_REQUEST_OUT_OF_RANGE;
    }

    pparams->udid  = udid;
    pparams->uaddr = did_map->memory_address;
    pparams->usz   = did_map->data_size;

    return NRC_POSITIVE_RESPONSE;
}

EXTERNC EXPORT uds_rdbi_result_t uds_rdbi(struct uds_state       *puds,
                                          const uds_rdbi_params_t params)
{
    uds_rdbi_result_t res;
    memset(&res, 0, sizeof(res));

    if (!puds) {
        res.rc = NRC_CONDITIONS_NOT_CORRECT;
        return res;
    }

    // Allocate memory for read data
    res.pdata = (uint8_t *)calloc(params.usz, sizeof(uint8_t));
    if (!res.pdata) {
        res.rc = NRC_CONDITIONS_NOT_CORRECT;
        return res;
    }

    // Read memory
    res.usz = __read_memory(params.uaddr, params.usz, res.pdata);
    if (res.usz < 0) {
        free(res.pdata);
        res.pdata = NULL;
        res.rc    = NRC_CONDITIONS_NOT_CORRECT;
        return res;
    }

    res.rc   = NRC_POSITIVE_RESPONSE;
    res.udid = params.udid;

    return res;
}

EXTERNC EXPORT void uds_rdbi_pack(struct uds_state *puds, uds_rdbi_result_t res,
                                  struct can_message *presp)
{
    if (res.rc != NRC_POSITIVE_RESPONSE) {
        presp->pdata[0] = UDS_SID_NEGATIVE_RESPONSE;
        presp->pdata[1] = UDS_SID_READ_DATA_BY_IDENTIFIER;
        presp->pdata[2] = res.rc;
        presp->usz      = 3;
        return;
    }

    // Positive response: SID + DID + data
    presp->pdata[0] = UDS_SID_READ_DATA_BY_IDENTIFIER | 0x40;
    presp->pdata[1] = (res.udid >> 8) & 0xFF;
    presp->pdata[2] = res.udid & 0xFF;
    memcpy(&presp->pdata[3], res.pdata, res.usz);
    presp->usz = 3 + res.usz;

    // cleanup
    if (res.pdata) {
        free(res.pdata);
        res.pdata = NULL;
    }
}
