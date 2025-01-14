#include <stdio.h>
#include <string.h>

#define HOST_SIZE (256)
#define API_KEY_SIZE (256)
#define BUFFER_SIZE (512)
#define HOST_SET (1)
#define PORT_SET (2)
#define API_KEY_SET (4)

#define CFG_FILE "/home/user/Documentos/GitHub/Mock-Project/build/config/config.yml"

// Macro for debug messages
#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, "[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) (void)0
#endif

typedef struct config {
    unsigned set;
    char host[HOST_SIZE];
    int port;
    char api_key[API_KEY_SIZE];
} CONFIG;

CONFIG config[1];

// Parse the buffer for config info. Return an error code or 0 for no error.
int parse_config(char *buf) {
    if (buf[0] == '\n' || buf[0] == '\0') {
        DEBUG_PRINT("Blank line detected, skipping.");
        return 0; // Blank line
    }

    if (buf[0] == '#') {
        DEBUG_PRINT("Comment line detected, skipping.");
        return 0; // Comment
    }

    if (sscanf(buf, " host: %255s", config->host) == 1) {
        if (config->set & HOST_SET) {
            DEBUG_PRINT("Error: Host already set.");
            return HOST_SET; // Error; host already set
        }
        config->set |= HOST_SET;
        DEBUG_PRINT("Host set to: %s", config->host);
        return 0;
    }

    if (sscanf(buf, " port: %d", &config->port) == 1) {
        if (config->set & PORT_SET) {
            DEBUG_PRINT("Error: Port already set.");
            return PORT_SET; // Error; port already set
        }
        config->set |= PORT_SET;
        DEBUG_PRINT("Port set to: %d", config->port);
        return 0;
    }

    if (sscanf(buf, " api_key: %255s", config->api_key) == 1) {
        if (config->set & API_KEY_SET) {
            DEBUG_PRINT("Error: API key already set.");
            return API_KEY_SET; // Error; API key already set
        }
        config->set |= API_KEY_SET;
        DEBUG_PRINT("API Key set to: %s", config->api_key);
        return 0;
    }

    DEBUG_PRINT("Syntax error in line: %s", buf);
    return 3; // Syntax error
}

void init_config() {
    DEBUG_PRINT("Initializing config.");
    config->set = 0u;

    FILE *f = fopen(CFG_FILE, "r");
    if (!f) {
        perror("Error opening config file");
        return;
    }

    char buf[BUFFER_SIZE];
    int line_number = 0;

    while (fgets(buf, sizeof(buf), f)) {
        ++line_number;
        int err = parse_config(buf);
        if (err) {
            fprintf(stderr, "Error in line %d: %d\n", line_number, err);
            DEBUG_PRINT("Error code %d at line %d", err, line_number);
        }
    }

    fclose(f);
    DEBUG_PRINT("Finished reading config file.");
}

void print_config() {
    printf("[host=%s, port=", config->set & HOST_SET ? config->host : "<unset>");

    if (config->set & PORT_SET) {
        printf("%d", config->port);
    } else {
        printf("<unset>");
    }

    printf(", apikey=");

    if (config->set & API_KEY_SET) {
        printf("%s]\n", config->api_key);
    } else {
        printf("<unset>]\n");
    }
}