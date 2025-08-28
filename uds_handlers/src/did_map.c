#include "did_map.h"

did_mapping_t DATA_IDENTIFIER_MAP[MAX_DID + 1] = {0};

void init_mapping(void)
{
    // TODO: we need a mechanism to configure this dids
    // Initialize all DIDs as invalid first
    for (int i = 0; i <= MAX_DID; i++) {
        DATA_IDENTIFIER_MAP[i].valid = 0;
    }

    // default
    DATA_IDENTIFIER_MAP[0x0000].memory_address = 0x00;
    DATA_IDENTIFIER_MAP[0x0000].data_size      = 2;
    DATA_IDENTIFIER_MAP[0x0000].access_level   = 0;
    DATA_IDENTIFIER_MAP[0x0000].valid          = 1;

    // Configure VIN
    DATA_IDENTIFIER_MAP[0xF190].memory_address = 0xFF;
    DATA_IDENTIFIER_MAP[0xF190].data_size      = 17;
    DATA_IDENTIFIER_MAP[0xF190].access_level   = 0;
    DATA_IDENTIFIER_MAP[0xF190].valid          = 1;
}
