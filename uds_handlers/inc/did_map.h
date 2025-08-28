#ifndef __DID_MAP_H__
#define __DID_MAP_H__

#include <stdint.h>

#define MAX_DID 0xFFFF

typedef struct {
    uint32_t memory_address;
    uint32_t data_size;
    uint8_t  access_level;
    uint8_t  valid;       
} did_mapping_t;

extern did_mapping_t DATA_IDENTIFIER_MAP[MAX_DID + 1];

void wrib_init_mapping(void);

#endif // __DID_MAP_H__
