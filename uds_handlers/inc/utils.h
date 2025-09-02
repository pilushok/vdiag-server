#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>
#include <stdint.h>
#include <memory.h>

uint8_t check_memrange(uint32_t uminaddr, uint32_t umaxaddr,
                              uint32_t uaddr, uint32_t sz);

int32_t write_memory(const char *mem_filename, uint32_t uaddr, uint32_t usz,
                     const uint8_t *pdata);

int32_t read_memory(const char *mem_filename, uint32_t uaddr, uint32_t usz,
                    uint8_t *pdata);
#endif // !__UTILS_H__
