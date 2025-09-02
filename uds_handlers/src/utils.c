#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "utils.h"

uint8_t check_memrange(uint32_t uminaddr, uint32_t umaxaddr,
                              uint32_t uaddr, uint32_t usz)
{
    return uaddr >= uminaddr && uaddr + usz <= umaxaddr;
}

int32_t write_memory(const char *mem_filename, uint32_t uaddr, uint32_t usz,
                     const uint8_t *pdata)
{
    int32_t memfd    = open(mem_filename, O_WRONLY);
    int32_t iwritesz = 0;

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

int32_t read_memory(const char *mem_filename, uint32_t uaddr, uint32_t usz,
                    uint8_t *pdata)
{
    int32_t memfd   = open(mem_filename, O_RDONLY);
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
