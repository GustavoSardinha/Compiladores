#ifndef ANALISADOR_SINTATICO_H
#define ANALISADOR_SINTATICO_H

#include "definicoes.h"
#include "analisador.h"

extern Token token;

int getQntErros();
void match(int esperado, const int *sync, const char *msg, int size);
void sincronizar(const char *msg, const int *sync, int size);
int esta_no_conjunto(int tk, const int *set, int size);

void programa(void);
void decls(void);
void decl(void);
void tipo(void);
void comandos(void);
void comando(void);
void entrada(void);
void saida(void);
void if_stmt(void);
void else_opt(void);
void while_stmt(void);
void bloco(void);
void args(void);
int expr_list(void);
int expr(void);
int expr2(void);
int term(void);
int term2(void);
int factor(void);
#endif 