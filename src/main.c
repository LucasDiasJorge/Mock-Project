#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>

#include "includes/config.h"
#include "includes/pi.h"

// Macro para mensagens de debug
#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) fprintf(stderr, "[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...) // Nada
#endif

pthread_t thread_pi;

// Função para tratar sinais
void handle_signal(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        fprintf(stdout, "\nSignal received, stopping...\n");
        exit(0);
    }
}

int main(int argc, char* argv[]) {

    // Verificação do número de argumentos
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive integer>\n", argv[0]);
        return 1;
    }

    // Configuração de captura de sinais
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1 || sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("Error setting up signal handler");
        return 1;
    }

    DEBUG_PRINT("Init main.");

    CONFIG config[1];
    init_config(config);

    DEBUG_PRINT("Printing config.");
    print_config(config);

    DEBUG_PRINT("Creating Threads.");

    int arg = atoi(argv[1]);
    if (pthread_create(&thread_pi, NULL, calc_pi, &arg) != 0) {
        fprintf(stderr, "Error creating thread for calc_pi\n");
        return 1;
    }

    DEBUG_PRINT("Joining Threads.");

    // Aguarda threads finalizarem
    pthread_join(thread_pi, NULL);

    fprintf(stdout, "Program terminated gracefully.\n");

    return 0;
}
