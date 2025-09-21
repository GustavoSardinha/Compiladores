#include <stdio.h>
#include <stdlib.h>
#include "analisador.h"

extern char *code; // vari�vel global alocada em analisador.c

int main() {
    analisar_arquivo("teste.txt");

    Token token;
    do {
        token = proximo_token();
    } while (token.nome_token != EOF);

    free(code); // libera mem�ria
    return 0;
}

