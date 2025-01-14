#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>

#define CONFIG_SIZE (256)
#define HOST_SET (1)
#define PORT_SET (2)

typedef struct config
{
    unsigned set;
    char host[CONFIG_SIZE];
    int port;
} CONFIG;

extern CONFIG config[1];

// Inicializa a estrutura de configuração.
void init_config();

// Analisa o buffer para informações de configuração. Retorna um código de erro ou 0 para nenhum erro.
int parse_config(char *buf);

// Imprime as informações de configuração no formato adequado.
void print_config();

#endif // CONFIG_H
