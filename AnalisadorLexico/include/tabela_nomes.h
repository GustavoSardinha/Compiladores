
#define TAM 1000

typedef struct Simbolo {
    char nome[50];
    char classe;
    char tipo;
} Simbolo;

extern Simbolo* tabela[TAM];
extern int nomes;

void inserir_identificador(const char* nome);
void inserir_palavra_reservada(const char* nome);
int buscar(const char* nome);
void declarar_tipo(const int idx, int tipo);
char get_tipo(const int idx);
char* get_nome(const int idx);