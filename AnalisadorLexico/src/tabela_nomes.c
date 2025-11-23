#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definicoes.h"

#define TAM 1000  
int nomes = 0;

typedef struct Simbolo {
    char nome[50];
    char classe;
    int declarada;
    char tipo;
} Simbolo;

Simbolo* tabela[TAM];  

void inserir_identificador(const char* nome) {
    Simbolo* novo = (Simbolo*) malloc(sizeof(Simbolo));
    strcpy(novo->nome, nome);
    novo->classe = 'i';
    novo->tipo = 'n';
    tabela[nomes] = novo;
    nomes++;
}
void inserir_palavra_reservada(const char* nome) {
    Simbolo* novo = (Simbolo*) malloc(sizeof(Simbolo));
    strcpy(novo->nome, nome);
    novo->classe = 's';
    tabela[nomes] = novo;
    nomes++;
}
int buscar(const char* nome) {
    for(int i = 0; i < TAM; i++){
        Simbolo* s = tabela[i];
        if(s != NULL){
            if(strcmp(s->nome, nome) == 0){
                if(s->classe == 'i')
                    return i;
                else
                    return -2;
            }
        }
    }
    return -1;
}
void declarar_tipo(const int idx, int tipo){
    switch (tipo)
    {
    case INT_TK:
        tabela[idx]->tipo = 'i';
        break;
    case FLOAT_TK:
        tabela[idx]->tipo = 'f';
        break;
    case STRING_TK:
        tabela[idx]->tipo = 's';
        break;
    default:
        tabela[idx]->tipo = 'n';
        break;
    }
}
char get_tipo(const int idx){
    return tabela[idx]->tipo;
}
char* get_nome(const int idx){
    return tabela[idx]->nome;
}

