#include "did_map.h"
#include <memory.h>
#include <stdio.h>

did_mapping_t DATA_IDENTIFIER_MAP[MAX_DID + 1] = {0};

void init_mapping(ecu_config_t ecucfg)
{
    memset(DATA_IDENTIFIER_MAP, 0, sizeof(DATA_IDENTIFIER_MAP));

    printf("Initializing %d DIDs from ini file...\n", ecucfg.did_count);

    for (int i = 0; i < ecucfg.did_count; i++) {
        if (ecucfg.data_identifiers[i].udid <= MAX_DID) {
            DATA_IDENTIFIER_MAP[ecucfg.data_identifiers[i].udid].memory_address =
                ecucfg.data_identifiers[i].address;
            DATA_IDENTIFIER_MAP[ecucfg.data_identifiers[i].udid].data_size =
                ecucfg.data_identifiers[i].size;
            DATA_IDENTIFIER_MAP[ecucfg.data_identifiers[i].udid].access_level =
                ecucfg.data_identifiers[i].access_level;
            DATA_IDENTIFIER_MAP[ecucfg.data_identifiers[i].udid].valid = true;

            printf("didcfgured DID 0x%04X (%s): address=0x%04X, size=%d, "
                   "access=%d\n",
                   ecucfg.data_identifiers[i].udid, ecucfg.did_names[i],
                   ecucfg.data_identifiers[i].address,
                   ecucfg.data_identifiers[i].size,
                   ecucfg.data_identifiers[i].access_level);
        } else {
            fprintf(stderr,
                    "Error: DID %s (0x%04X) exceeds maximum allowed value\n",
                    ecucfg.did_names[i], ecucfg.data_identifiers[i].udid);
        }
    }
}
