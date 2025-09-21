//NOME TOKENS
#define IF 256
#define THEN 257
#define ELSE 258
#define RELOP 259
#define ID 260
#define NUM 261

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

