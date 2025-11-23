#include <string.h>

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
#define MAIS            281
#define MENOS           282
#define PRODUTO         283
#define DIVISAO         284

typedef struct {
    int nome_token;
    int atributo;
} Token;

int filtrar_palavra_reservada(char* nome) {
    if (strcmp(nome, "inicio") == 0) return INICIO;
    if (strcmp(nome, "fim") == 0) return FIM;
    if (strcmp(nome, "if") == 0) return IF;
    if (strcmp(nome, "then") == 0) return THEN;
    if (strcmp(nome, "else") == 0) return ELSE;
    if (strcmp(nome, "while") == 0) return WHILE;
    if (strcmp(nome, "print") == 0) return PRINT;
    if (strcmp(nome, "read") == 0) return READ;
    if (strcmp(nome, "int") == 0) return INT_TK;
    if (strcmp(nome, "float") == 0) return FLOAT_TK;
    if (strcmp(nome, "string") == 0) return STRING_TK;
    return ID;
}
const char* nome_do_token(int tk) {
    switch(tk) {
        case INICIO: return "inicio";
        case FIM: return "fim";
        case IF: return "if";
        case THEN: return "then";
        case ELSE: return "else";
        case WHILE: return "while";
        case PRINT: return "print";
        case READ: return "read";
        case INT_TK: return "int";
        case FLOAT_TK: return "float";
        case STRING_TK: return "string";
        case ID: return "identificador";

        case INT: return "inteiro";
        case FLOAT: return "float literal";
        case STRING: return "string literal";

        case OP: return "operador";
        case VIRGULA: return "virgula";
        case PONTO_VIRGULA: return "ponto e virgula";
        case PARENTESES_ESQ: return "(";
        case PARENTESES_DIR: return ")";
        case BLOCO_ESQ: return "{";
        case BLOCO_DIR: return "}";
        case COLCHETE_ESQ: return "[";
        case COLCHETE_DIR: return "]";
        case EQ: return "=";

        case EF: return "fim de arquivo";

        default: return "token desconhecido";
    }
}