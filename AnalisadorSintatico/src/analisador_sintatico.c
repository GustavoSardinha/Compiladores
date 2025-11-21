#include "analisador.h"
#include "analisador_sintatico.h"
#include <stdio.h>
#include <stdlib.h>

Token token;

int esta_no_conjunto(int tk, const int *set, int size) {
    for (int i = 0; i < size; i++)
        if (tk == set[i]) return 1;
    return 0;
}
void sincronizar(const char *erro, const int *sync, int size) {
    while (!esta_no_conjunto(token.nome_token, sync, size) &&
           token.nome_token != EF) {
        printf("\n[ERRO] linha %d: %s. Encontrado: %s\n",
           getLinha(), erro, nome_do_token(token.nome_token));
        token = proximo_token();
    }
}

void match(int esperado) {
    if (token.nome_token == esperado) {
        token = proximo_token();
        return;
    }

    printf("\n[ERRO] linha %d: esperado %s, encontrado %s\n",
           getLinha(), nome_do_token(esperado),
           nome_do_token(token.nome_token));

    const int sync[] = { esperado, PONTO_VIRGULA, BLOCO_DIR, FIM, EF };
    sincronizar("Erro de casamento de token", sync, 5);

    if (token.nome_token == esperado)
        token = proximo_token();
}

void programa(void) {
    token = proximo_token();

    const int sync_programa[] = { INICIO, EF };
    if (token.nome_token != INICIO)
        sincronizar("esperado inicio", sync_programa, 2);

    match(INICIO);
    decls();
    comandos();
    match(FIM);

    while (token.nome_token != EF) {
        printf("\n[ERRO] linha %d: tokens extra após fim: %s\n",
               getLinha(), nome_do_token(token.nome_token));
        token = proximo_token();
    }
}

void decls(void) {
    while (token.nome_token == INT_TK ||
           token.nome_token == FLOAT_TK ||
           token.nome_token == STRING_TK)
        decl();
}

void decl(void) {
    const int sync[] = {
        INT_TK, FLOAT_TK, STRING_TK, ID, READ, PRINT, IF, WHILE, BLOCO_ESQ, FIM
    };

    if (!(token.nome_token == INT_TK ||
          token.nome_token == FLOAT_TK ||
          token.nome_token == STRING_TK))
        sincronizar("tipo esperado", sync, 10);

    tipo();
    match(ID);
    match(PONTO_VIRGULA);
}

void tipo(void) {
    const int sync[] = { ID, FIM };
    if (token.nome_token == INT_TK) match(INT_TK);
    else if (token.nome_token == FLOAT_TK) match(FLOAT_TK);
    else if (token.nome_token == STRING_TK) match(STRING_TK);
    else sincronizar("tipo invalido", sync, 2);
}

void comandos(void) {
    while (token.nome_token == ID ||
           token.nome_token == READ ||
           token.nome_token == PRINT ||
           token.nome_token == IF ||
           token.nome_token == WHILE ||
           token.nome_token == BLOCO_ESQ)
        comando();
}

void comando(void) {
    const int sync[] = { ID, READ, PRINT, IF, WHILE, BLOCO_ESQ, BLOCO_DIR, FIM };

    if (token.nome_token == ID) {
        match(ID);
        if (token.nome_token == EQ) {
            match(EQ);
            expr();
            match(PONTO_VIRGULA);
        } else if (token.nome_token == PARENTESES_ESQ) {
            match(PARENTESES_ESQ);
            args();
            match(PARENTESES_DIR);
            match(PONTO_VIRGULA);
        } else {
            sincronizar("esperado '=' ou '('", sync, 8);
        }
    }
    else if (token.nome_token == READ) entrada();
    else if (token.nome_token == PRINT) saida();
    else if (token.nome_token == IF) if_stmt();
    else if (token.nome_token == WHILE) while_stmt();
    else if (token.nome_token == BLOCO_ESQ) bloco();
    else sincronizar("comando invalido", sync, 8);
}

void entrada(void) {
    match(READ);
    match(PARENTESES_ESQ);
    match(ID);
    match(PARENTESES_DIR);
    match(PONTO_VIRGULA);
}

void saida(void) {
    match(PRINT);
    match(PARENTESES_ESQ);
    expr();
    match(PARENTESES_DIR);
    match(PONTO_VIRGULA);
}

void if_stmt(void) {
    match(IF);
    match(PARENTESES_ESQ);
    expr();
    match(PARENTESES_DIR);
    comando();
    else_opt();
}

void else_opt(void) {
    if (token.nome_token == ELSE) {
        match(ELSE);
        comando();
    }
}

void while_stmt(void) {
    match(WHILE);
    match(PARENTESES_ESQ);
    expr();
    match(PARENTESES_DIR);
    comando();
}

void bloco(void) {
    match(BLOCO_ESQ);
    comandos();
    match(BLOCO_DIR);
}

void args(void) {
    if (token.nome_token != PARENTESES_DIR)
        expr_list();
}

void expr_list(void) {
    expr();
    while (token.nome_token == VIRGULA) {
        match(VIRGULA);
        expr();
    }
}

void expr(void) {
    term();
    expr2();
}

void expr2(void) {
    while (token.nome_token == OP &&
          (token.atributo == MAIS || token.atributo == MENOS)) {
        match(OP);
        term();
    }
}

void term(void) {
    factor();
    term2();
}

void term2(void) {
    while (token.nome_token == OP &&
          (token.atributo == PRODUTO || token.atributo == DIVISAO)) {
        match(OP);
        factor();
    }
}

void factor(void) {
    const int sync[] = { VIRGULA, PARENTESES_DIR, PONTO_VIRGULA, FIM };

    if (token.nome_token == ID) match(ID);
    else if (token.nome_token == INT ||
             token.nome_token == FLOAT ||
             token.nome_token == STRING)
        match(token.nome_token);
    else if (token.nome_token == PARENTESES_ESQ) {
        match(PARENTESES_ESQ);
        expr();
        match(PARENTESES_DIR);
    }
    else if (token.nome_token == OP && token.atributo == MENOS) {
        match(OP);
        factor();
    }
    else sincronizar("fator invalido", sync, 4);
}
