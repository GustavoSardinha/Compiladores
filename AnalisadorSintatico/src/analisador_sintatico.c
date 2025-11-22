#include "analisador.h"
#include "analisador_sintatico.h"
#include <stdio.h>
#include <stdlib.h>

Token token;
int erros = 0;

int getQntErros(){
    return erros;
}
int esta_no_conjunto(int tk, const int *set, int size) {
    for (int i = 0; i < size; i++)
        if (tk == set[i]) return 1;
    return 0;
}

void sincronizar(const char *msg, const int *sync, int size) {
    printf("\n[ERRO] linha %d: %s. Encontrado: %s\n",
           getLinha(), msg, nome_do_token(token.nome_token));
    if (token.nome_token == EF) return;
    token = proximo_token();
    while (!esta_no_conjunto(token.nome_token, sync, size) &&
           token.nome_token != EF) {
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
    sincronizar("Erro", sync, 5);
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
    if (token.nome_token != FIM)
        sincronizar("esperado fim", sync_programa, 2);
}

void decls(void) {
    while (token.nome_token == INT_TK ||
           token.nome_token == FLOAT_TK ||
           token.nome_token == STRING_TK)
        decl();
}

void decl(void) {
    const int sync[] = {
        INT_TK, FLOAT_TK, STRING_TK, ID, READ, PRINT,
        IF, WHILE, BLOCO_ESQ, FIM, EF
    };
    if (!(token.nome_token == INT_TK ||
          token.nome_token == FLOAT_TK ||
          token.nome_token == STRING_TK))
        sincronizar("tipo esperado", sync, 11);
    if (token.nome_token == INT_TK || token.nome_token == FLOAT_TK || token.nome_token == STRING_TK)
        tipo();
    match(ID);
    match(PONTO_VIRGULA);
}

void tipo(void) {
    const int sync[] = { ID, FIM, EF };
    if (token.nome_token == INT_TK) match(INT_TK);
    else if (token.nome_token == FLOAT_TK) match(FLOAT_TK);
    else if (token.nome_token == STRING_TK) match(STRING_TK);
    else sincronizar("tipo invalido", sync, 3);
}

void comandos(void) {
    while (1) {
        if (token.nome_token == BLOCO_DIR) return;
        if (token.nome_token == FIM) return;
        if (token.nome_token == EF) return;
        if (token.nome_token == ID ||
            token.nome_token == READ ||
            token.nome_token == PRINT ||
            token.nome_token == IF ||
            token.nome_token == WHILE ||
            token.nome_token == BLOCO_ESQ ||
            token.nome_token == ELSE) {
            comando();
        } else {
            return;
        }
    }
}

void comando(void) {
    const int sync[] = {
        ID, READ, PRINT, IF, WHILE, BLOCO_ESQ, BLOCO_DIR, FIM, ELSE, EF
    };
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
            sincronizar("esperado '=' ou '('", sync, 10);
        }
        return;
    }
    if (token.nome_token == READ) { entrada(); return; }
    if (token.nome_token == PRINT) { saida(); return; }
    if (token.nome_token == IF) { if_stmt(); return; }
    if (token.nome_token == WHILE) { while_stmt(); return; }
    if (token.nome_token == BLOCO_ESQ) { bloco(); return; }
    sincronizar("comando invalido", sync, 10);
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
    const int sync[] = {
        VIRGULA, PARENTESES_DIR, PONTO_VIRGULA, FIM, EF
    };
    if (token.nome_token == ID) { match(ID); return; }
    if (token.nome_token == INT ||
        token.nome_token == FLOAT ||
        token.nome_token == STRING) { match(token.nome_token); return; }
    if (token.nome_token == PARENTESES_ESQ) { match(PARENTESES_ESQ); expr(); match(PARENTESES_DIR); return; }
    if (token.nome_token == OP && token.atributo == MENOS) { match(OP); factor(); return; }
    sincronizar("fator invalido", sync, 5);
}
