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

extern ItemValor* tabela_valores[TAM];
extern int valores;

int inserir_valor(const ItemValor* val);