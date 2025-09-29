#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 1000

typedef union Valor
{
    int valor_inteiro;
    char* valor_string;
    float valor_float;
} Valor;
typedef struct ItemValor{
    Valor v;
    char classe;
} ItemValor;

ItemValor* tabela_valores[TAM];
int valores;

int inserir_valor(ItemValor* val) {
    if (valores >= TAM) {
        printf("Erro: tabela de valores cheia!\n");
        return -1;
    }

    ItemValor* novo = malloc(sizeof(ItemValor));
    if (!novo) {
        printf("Erro ao alocar memória para valor!\n");
        return -1;
    }
    *novo = *val;

    tabela_valores[valores] = novo;
    return valores++;
}
