#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 50  

typedef struct Simbolo {
    char nome[50];
    char tipo[20];
    char valor[20];
    struct Simbolo* prox;
} Simbolo;

Simbolo* tabela[TAM];  

unsigned int hash(const char *str) {
    unsigned int h = 0;
    while (*str) {
        h = (h * 31) + *str++;
    }
    return h % TAM;
}

void inserir(const char* nome, const char* tipo, const char* valor) {
    unsigned int idx = hash(nome);

    Simbolo* novo = (Simbolo*) malloc(sizeof(Simbolo));
    strcpy(novo->nome, nome);
    strcpy(novo->tipo, tipo);
    strcpy(novo->valor, valor);
    novo->prox = tabela[idx];
    tabela[idx] = novo;
}

Simbolo* buscar(const char* nome) {
    unsigned int idx = hash(nome);
    Simbolo* atual = tabela[idx];
    while (atual) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}
