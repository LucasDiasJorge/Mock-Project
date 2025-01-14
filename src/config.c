#include <stdio.h>
#include <string.h>

#define CONFIG_SIZE (256)
#define HOST_SET (1)
#define PORT_SET (2)

#define CFG_FILE "/home/user/Documentos/GitHub/CKP-Project/build/config/config.yml"

// Macro para mensagens de debug
#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) fprintf(stderr, "[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...) // Nada
#endif

typedef struct config
{
    unsigned set;
    char host[CONFIG_SIZE];
    int port;
} CONFIG;

// Parse the buffer for config info. Return an error code or 0 for no error.
int parse_config(char *buf, CONFIG *config)
{
    char dummy[CONFIG_SIZE];
    if (sscanf(buf, " %s", dummy) == EOF) {
        DEBUG_PRINT("Blank line detected, skipping.");
        return 0; // blank line
    }

    if (sscanf(buf, " %[#]", dummy) == 1) {
        DEBUG_PRINT("Comment line detected, skipping.");
        return 0; // comment
    }

    if (sscanf(buf, " host: %s", config->host) == 1) {
        if (config->set & HOST_SET) {
            DEBUG_PRINT("Error: host already set.");
            return HOST_SET; // error; host already set
        }
        config->set |= HOST_SET;
        DEBUG_PRINT("Host set to: %s", config->host);
        return 0;
    }

    if (sscanf(buf, " port: %d", &config->port) == 1) {
        if (config->set & PORT_SET) {
            DEBUG_PRINT("Error: port already set.");
            return PORT_SET; // error; port already set
        }
        config->set |= PORT_SET;
        DEBUG_PRINT("Port set to: %d", config->port);
        return 0;
    }

    DEBUG_PRINT("Syntax error in line: %s", buf);
    return 3; // syntax error
}

void init_config(CONFIG *config)
{
    DEBUG_PRINT("Initializing config.");

    config->set = 0u;

    FILE *f = fopen(CFG_FILE, "r");
    if (!f) {
        fprintf(stderr, "Error: could not open config file %s\n", CFG_FILE);
        return;
    }

    char buf[CONFIG_SIZE];
    int line_number = 0;

    while (fgets(buf, sizeof buf, f)) {
        ++line_number;
        int err = parse_config(buf, config);
        if (err) {
            fprintf(stderr, "Error in line %d: %d\n", line_number, err);
            DEBUG_PRINT("Error code %d at line %d", err, line_number);
        }
    }

    fclose(f);
    DEBUG_PRINT("Finished reading config file.");
}

void print_config(CONFIG *config)
{

    printf("[host=%s,port=", config->set & HOST_SET ? config->host : "<unset>");

    if (config->set & PORT_SET)
        printf("%d]\n", config->port);
    else
        printf("<unset>]");
}
