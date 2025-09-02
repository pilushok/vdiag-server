#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>
#include <stdbool.h>

#define MAX_STRING_LENGTH 256
#define MAX_DID_ENTRIES 50

typedef struct {
    char     interface[MAX_STRING_LENGTH];
    uint32_t tx_id;
    uint32_t rx_id;
    char     protocol[MAX_STRING_LENGTH];
} can_config_t;

typedef struct {
    char     file_path[MAX_STRING_LENGTH];
    uint32_t start_addr;
    uint32_t end_addr;
} memory_config_t;

typedef struct {
    uint32_t address;
    uint16_t size;
    uint8_t  access_level;
} data_identifiers_t;

typedef struct {
    char level[MAX_STRING_LENGTH];
    char file[MAX_STRING_LENGTH];
} log_config_t;

typedef struct {
    can_config_t       can;
    memory_config_t    memory;
    data_identifiers_t data_identifiers[MAX_DID_ENTRIES];
    char               did_names[MAX_DID_ENTRIES][8];
    int                did_count;
    log_config_t       logging;
} ecu_config_t;

int32_t parse_config(const char *filename, ecu_config_t *config);

void print_config(const ecu_config_t *config);

#endif // __CONFIG_H__
