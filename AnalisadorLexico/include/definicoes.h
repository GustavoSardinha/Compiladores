//NOME TOKENS
#define IF 256
#define THEN 257
#define ELSE 258
#define RELOP 259
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
#define LT 262
#define LE 263
#define EQ 264
#define NE 265
#define GT 266
#define GE 267

typedef struct {
    int nome_token;
    int atributo;
} Token;

