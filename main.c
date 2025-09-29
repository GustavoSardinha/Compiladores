#include <stdio.h>
#include <stdlib.h>
#include "analisador.h"
#include "tabela_nomes.h"
#include <locale.h>

extern char *code; 

int main() {
    analisar_arquivo("teste.txt");
    setlocale(LC_ALL, "");
    inserir_palavra_reservada("inicio");
    inserir_palavra_reservada("fim");
    inserir_palavra_reservada("int");
    inserir_palavra_reservada("float");
    inserir_palavra_reservada("string");
    inserir_palavra_reservada("read");
    inserir_palavra_reservada("print");
    inserir_palavra_reservada("if");
    inserir_palavra_reservada("else");
    inserir_palavra_reservada("while");
    Token token;
    do {
        token = proximo_token();
    } while (token.nome_token != EF);

    free(code);
    return 0;
}

