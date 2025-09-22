#include <stdio.h>
#include <stdlib.h>
#include "analisador.h"

extern char *code; 

int main() {
    analisar_arquivo("teste.txt");

    Token token;
    do {
        token = proximo_token();
    } while (token.nome_token != EF);

    free(code);
    return 0;
}

