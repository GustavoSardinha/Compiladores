// NOME TOKENS
#define INICIO          256
#define FIM             257
#define IF              258
#define THEN            259
#define ELSE            260
#define WHILE           261
#define PRINT           262
#define READ            263
#define INT_TK          264   
#define FLOAT_TK        265   
#define STRING_TK       266   
#define ID              267
#define INT       268
#define FLOAT     269
#define STRING         270
#define OP              271
#define VIRGULA         272
#define PONTO_VIRGULA   273
#define PARENTESES_ESQ  274
#define PARENTESES_DIR  275
#define BLOCO_ESQ       276
#define BLOCO_DIR       277
#define COLCHETE_ESQ    278
#define COLCHETE_DIR    279
#define EQ 280
#define EF             -1

// ATRIBUTOS de OP
#define MAIS            280
#define MENOS           281
#define PRODUTO         282
#define DIVISAO         283

typedef struct {
    int nome_token;
    int atributo;
} Token;

int filtrar_palavra_reservada(char* nome);