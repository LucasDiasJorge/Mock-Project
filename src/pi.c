#include <stdio.h>
#include <math.h>

// Função para calcular PI usando a série de Leibniz
void *calc_pi(void *arg){

    int n = *(int *)arg;  // Convertendo o argumento para o tipo correto

    double pi = 0.0;
    for (int i = 0; i < n; i++) {
        // Fórmula: π/4 = 1 - 1/3 + 1/5 - 1/7 + ...
        double termo = pow(-1, i) / (2 * i + 1);
        pi += termo;
    }

    printf("Result of PI: %f\n",pi*4);

    return NULL;
}