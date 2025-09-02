#include "ecu_config.h"
#include "inih/ini.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static uint32_t parse_hex(const char *str)
{
    return (uint32_t)strtoul(str, NULL, 16);
}

static char *trim(char *str)
{
    char *end;

    while (isspace((unsigned char)*str))
        str++;
    if (*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    *(end + 1) = '\0';

    return str;
}

static int32_t handler(void *user, const char *section, const char *name,
                       const char *value)
{
    ecu_config_t *config = (ecu_config_t *)user;

    // CAN section
    if (strcasecmp(section, "can") == 0) {
        if (strcasecmp(name, "interface") == 0) {
            strncpy(config->can.interface, value,
                    sizeof(config->can.interface));
        } else if (strcasecmp(name, "tx_id") == 0) {
            config->can.tx_id = parse_hex(value);
        } else if (strcasecmp(name, "rx_id") == 0) {
            config->can.rx_id = parse_hex(value);
        } else if (strcasecmp(name, "protocol") == 0) {
            strncpy(config->can.protocol, value, sizeof(config->can.protocol));
        }
    }

    // Memory section
    else if (strcasecmp(section, "memory") == 0) {
        if (strcasecmp(name, "file_path") == 0) {
            strncpy(config->memory.file_path, value,
                    sizeof(config->memory.file_path));
        } else if (strcasecmp(name, "start_addr") == 0) {
            config->memory.start_addr = parse_hex(value);
        } else if (strcasecmp(name, "end_addr") == 0) {
            config->memory.end_addr = parse_hex(value);
        }
    }

    // Data identifiers section
    else if (strcasecmp(section, "data_identifiers") == 0) {
        if (config->did_count < MAX_DID_ENTRIES) {
            char *tokens[3];
            char *value_copy = strdup(value);
            char *token      = strtok(value_copy, ":");
            int   i          = 0;

            while (token != NULL && i < 3) {
                tokens[i++] = trim(token);
                token       = strtok(NULL, ":");
            }

            if (i == 3) {
                strncpy(config->did_names[config->did_count], name,
                        sizeof(config->did_names[0]));
                config->data_identifiers[config->did_count].address =
                    parse_hex(tokens[0]);
                config->data_identifiers[config->did_count].size =
                    (uint16_t)atoi(tokens[1]);
                config->data_identifiers[config->did_count].access_level =
                    (uint8_t)atoi(tokens[2]);
                config->did_count++;
            }

            free(value_copy);
        }
    }

    // Logging section
    else if (strcasecmp(section, "logging") == 0) {
        if (strcasecmp(name, "level") == 0) {
            strncpy(config->logging.level, value,
                    sizeof(config->logging.level));
        } else if (strcasecmp(name, "file") == 0) {
            strncpy(config->logging.file, value, sizeof(config->logging.file));
        }
    }

    return 1;
}

int32_t parse_config(const char *filename, ecu_config_t *config)
{
    memset(config, 0, sizeof(ecu_config_t));
    config->did_count = 0;

    // Parse the INI file
    if (ini_parse(filename, handler, config) < 0) {
        fprintf(stderr, "Error: Could not load configuration file '%s'\n",
                filename);
        return false;
    }

    return true;
}

void print_config(const ecu_config_t *config)
{
    printf("=== ECU Configuration ===\n\n");

    printf("[CAN]\n");
    printf("Interface: %s\n", config->can.interface);
    printf("TX ID: 0x%X\n", config->can.tx_id);
    printf("RX ID: 0x%X\n", config->can.rx_id);
    printf("Protocol: %s\n\n", config->can.protocol);

    printf("[Memory]\n");
    printf("File Path: %s\n", config->memory.file_path);
    printf("Start Address: 0x%04X\n", config->memory.start_addr);
    printf("End Address: 0x%04X\n\n", config->memory.end_addr);

    printf("[Data Identifiers]\n");
    for (int i = 0; i < config->did_count; i++) {
        printf("%s = 0x%04X:%d:%d\n", config->did_names[i],
               config->data_identifiers[i].address,
               config->data_identifiers[i].size,
               config->data_identifiers[i].access_level);
    }
    printf("\n");

    printf("[Logging]\n");
    printf("Level: %s\n", config->logging.level);
    printf("File: %s\n", config->logging.file);
}
