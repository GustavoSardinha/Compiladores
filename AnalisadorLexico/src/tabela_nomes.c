#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 1000  
int nomes = 0;

typedef struct Simbolo {
    char nome[50];
    char classe;
    int indice;
} Simbolo;

Simbolo* tabela[TAM];  

void inserir_identificador(const char* nome) {
    Simbolo* novo = (Simbolo*) malloc(sizeof(Simbolo));
    strcpy(novo->nome, nome);
    novo->classe = 'i';
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
