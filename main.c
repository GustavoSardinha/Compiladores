#include <stdio.h>
#include <stdlib.h>
#include "tabela_nomes.h"
#include "analisador_sintatico.h"

extern char *code; 

int main() {
    analisar_arquivo("teste.txt");
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
    programa();
    if(getQntErros() == 0)
        printf("Programa aceito!");
    free(code);
    return 0;
}

