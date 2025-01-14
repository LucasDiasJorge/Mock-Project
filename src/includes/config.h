#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>

#define HOST_SIZE (256)
#define API_KEY_SIZE (256)
#define BUFFER_SIZE (512)
#define HOST_SET (1)
#define PORT_SET (2)
#define API_KEY_SET (4)

typedef struct config
{
    unsigned set;
    char host[HOST_SIZE];
    int port;
    char api_key[256];
} CONFIG;

extern CONFIG config[1];

// Inicializa a estrutura de configuração.
void init_config();

// Analisa o buffer para informações de configuração. Retorna um código de erro ou 0 para nenhum erro.
int parse_config(char *buf);

// Imprime as informações de configuração no formato adequado.
void print_config();

#endif // CONFIG_H
