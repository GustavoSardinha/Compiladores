#define TAM 50 

typedef struct Simbolo {
    char nome[50];
    char tipo[20];
    char valor[20];
    struct Simbolo* prox;
} Simbolo;

Simbolo* tabela[TAM];
void inserir(const char* nome, const char* tipo, const char* valor);
Simbolo* buscar(const char* nome);