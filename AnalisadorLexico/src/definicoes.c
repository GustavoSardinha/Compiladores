#include <string.h>
//NOME TOKENS
#define IF 256
#define THEN 257
#define ELSE 258
#define EQ 259
#define ID 260
#define NUM 261
#define PONTO_VIRGULA 268
#define PARENTESES_ESQ 269
#define PARENTESES_DIR 270
#define BLOCO_ESQ 271
#define BLOCO_DIR 272
#define COLCHETE_ESQ 273
#define COLCHETE_DIR 274
#define EF -1

//ATRIBUTOS

typedef struct {
    int nome_token;
    int atributo;
} Token;

int filtrar_palavra_reservada(char* nome) {
    if (strcmp(nome, "if") == 0) return IF;
    if (strcmp(nome, "then") == 0) return THEN;
    if (strcmp(nome, "else") == 0) return ELSE;
    return ID;
}