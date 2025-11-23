#include "analisador.h"
#include "analisador_sintatico.h"
#include "tabela_nomes.h"
#include <stdio.h>
#include <stdlib.h>

#define TYPE_ERROR 0
#define TYPE_INT 1
#define TYPE_FLOAT 2
#define TYPE_STRING 3

Token token;
int erros = 0;

int getQntErros() {
    return erros;
}

int esta_no_conjunto(int tk, const int *set, int size) {
    for (int i = 0; i < size; i++)
        if (tk == set[i]) return 1;
    return 0;
}

void sincronizar(const char *msg, const int *sync, int size) {
    erros++;
    printf("[ERRO] linha %d: %s. Encontrado: %s\n",
           getLinha(), msg, nome_do_token(token.nome_token));
    while (!esta_no_conjunto(token.nome_token, sync, size) &&
           token.nome_token != EF) {
        token = proximo_token();
    }
}

void match(int esperado, const int *sync, const char *msg, int size) {
    if(token.nome_token == EF) return;
    if (token.nome_token == esperado) {
        token = proximo_token();
        return;
    }
    sincronizar(msg, sync, size);
}

int tipo_char_para_const(char t) {
    if (t == 'i') return TYPE_INT;
    if (t == 'f') return TYPE_FLOAT;
    if (t == 's') return TYPE_STRING;
    return TYPE_ERROR;
}

int tipo_literal_por_token(int tk) {
    if (tk == INT) return TYPE_INT;
    if (tk == FLOAT) return TYPE_FLOAT;
    if (tk == STRING) return TYPE_STRING;
    return TYPE_ERROR;
}

void programa(void) {
    token = proximo_token();
    const int sync_programa[] = { INICIO, EF };
    match(INICIO, sync_programa, "programa deve comecar com 'inicio'", 2);
    decls();
    comandos();
    match(FIM, sync_programa, "programa deve terminar com 'fim'", 2);
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
    int tipagem = token.nome_token;
    if (token.nome_token == INT_TK ||
        token.nome_token == FLOAT_TK ||
        token.nome_token == STRING_TK)
        tipo();
    int indice = token.atributo;
    match(ID, sync, "identificador esperado apos tipo", 11);
    match(PONTO_VIRGULA, sync, "ponto e virgula esperado ao final da declaracao", 11);
    if(get_tipo(indice) == 'n')
        declarar_tipo(indice, tipagem);
    else{
        erros++;
        printf("[ERRO] na linha %d, nome %s ja foi declarado\n", getLinha(), get_nome(indice));
    }
}

void tipo(void) {
    const int sync[] = { ID, FIM, EF };
    if (token.nome_token == INT_TK) match(INT_TK, sync, "tipo 'int' esperado", 3);
    else if (token.nome_token == FLOAT_TK) match(FLOAT_TK, sync, "tipo 'float' esperado", 3);
    else if (token.nome_token == STRING_TK) match(STRING_TK, sync, "tipo 'string' esperado", 3);
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

int expr(void);
int term(void);
int factor(void);

void comando(void) {
    const int sync[] = {
        ID, READ, PRINT, IF, WHILE, BLOCO_ESQ, BLOCO_DIR, FIM, ELSE, EF
    };

    if (token.nome_token == ID) {
        int indice = token.atributo;
        if(get_tipo(indice) == 'n'){
            erros++;
            printf("[ERRO] linha %d, tentativa de uso de variavel de nome %s antes de uma declaracao\n", getLinha(), get_nome(indice));
        }
        match(ID, sync, "identificador esperado no inicio do comando", 10);
        if (token.nome_token == EQ) {
            match(EQ, sync, "operador '=' esperado apos identificador", 10);
            int tipo_expr = expr();
            if (get_tipo(indice) != 'n') {
                int tipo_var = tipo_char_para_const(get_tipo(indice));
                if (tipo_var == TYPE_INT && tipo_expr == TYPE_FLOAT) {
                    erros++;
                    printf("[ERRO] linha %d: atribuicao invalida: variavel %s do tipo int nao pode receber float\n", getLinha(), get_nome(indice));
                }
                if (tipo_var == TYPE_STRING && (tipo_expr == TYPE_INT || tipo_expr == TYPE_FLOAT)) {
                    erros++;
                    printf("[ERRO] linha %d: atribuicao invalida: variavel %s do tipo string nao pode receber tipo numerico\n", getLinha(), get_nome(indice));
                }
            }
            match(PONTO_VIRGULA, sync, "ponto e virgula esperado ao final da atribuicao", 10);
        } else if (token.nome_token == PARENTESES_ESQ) {
            match(PARENTESES_ESQ, sync, "esperado '(' para chamada de funcao", 10);
            args();
            match(PARENTESES_DIR, sync, "esperado ')'", 10);
            match(PONTO_VIRGULA, sync, "ponto e virgula esperado apos chamada de funcao", 10);
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
    const int sync[] = { EF, FIM, PONTO_VIRGULA, PARENTESES_DIR, BLOCO_DIR };
    match(READ, sync, "esperado 'read'", 5);
    match(PARENTESES_ESQ, sync, "esperado '('", 5);
    match(ID, sync, "identificador esperado como parametro de read", 5);
    match(PARENTESES_DIR, sync, "esperado ')'", 5);
    match(PONTO_VIRGULA, sync, "ponto e virgula esperado", 5);
}

void saida(void) {
    const int sync[] = { EF, FIM, PONTO_VIRGULA, PARENTESES_DIR, BLOCO_DIR };
    match(PRINT, sync, "esperado 'print'", 5);
    match(PARENTESES_ESQ, sync, "esperado '('", 5);
    expr();
    match(PARENTESES_DIR, sync, "esperado ')'", 5);
    match(PONTO_VIRGULA, sync, "ponto e virgula esperado", 5);
}

void if_stmt(void) {
    const int sync[] = { ELSE, FIM, EF, BLOCO_DIR, PONTO_VIRGULA };
    match(IF, sync, "esperado 'if'", 5);
    match(PARENTESES_ESQ, sync, "esperado '('", 5);
    expr();
    match(PARENTESES_DIR, sync, "esperado ')'", 5);
    comando();
    else_opt();
}

void else_opt(void) {
    const int sync[] = { FIM, EF, BLOCO_DIR, PONTO_VIRGULA };
    if (token.nome_token == ELSE) {
        match(ELSE, sync, "esperado 'else'", 4);
        comando();
    }
}

void while_stmt(void) {
    const int sync[] = { FIM, EF, BLOCO_DIR, PONTO_VIRGULA };
    match(WHILE, sync, "esperado 'while'", 4);
    match(PARENTESES_ESQ, sync, "esperado '('", 4);
    expr();
    match(PARENTESES_DIR, sync, "esperado ')'", 4);
    comando();
}

void bloco(void) {
    const int sync[] = { EF, FIM, BLOCO_DIR };
    match(BLOCO_ESQ, sync, "esperado '{'", 3);
    comandos();
    match(BLOCO_DIR, sync, "esperado '}'", 3);
}

void args(void) {
    if (token.nome_token != PARENTESES_DIR)
        expr_list();
}

int expr_list(void) {
    int t = expr();
    while (token.nome_token == VIRGULA) {
        const int sync[] = { EF, FIM, PARENTESES_DIR };
        match(VIRGULA, sync, "virgula esperada entre expressoes", 3);
        t = expr();
    }
    return t;
}

int expr(void) {
    int left = term();
    while (token.nome_token == OP &&
           (token.atributo == MAIS || token.atributo == MENOS)) {
        const int sync[] = { EF, FIM, PARENTESES_DIR, PONTO_VIRGULA };
        match(OP, sync, "operador '+' ou '-' esperado", 4);
        int right = term();
        if (left == TYPE_STRING || right == TYPE_STRING) {
            erros++;
            printf("[ERRO] linha %d: operacao aritmetica invalida com string\n", getLinha());
            left = TYPE_ERROR;
        } else if (left == TYPE_FLOAT || right == TYPE_FLOAT) {
            left = TYPE_FLOAT;
        } else {
            left = TYPE_INT;
        }
    }
    return left;
}

int term(void) {
    int left = factor();
    while (token.nome_token == OP &&
           (token.atributo == PRODUTO || token.atributo == DIVISAO)) {
        const int sync[] = { EF, FIM, PARENTESES_DIR, PONTO_VIRGULA };
        match(OP, sync, "operador '*' ou '/' esperado", 4);
        int right = factor();
        if (left == TYPE_STRING || right == TYPE_STRING) {
            erros++;
            printf("[ERRO] linha %d: operacao aritmetica invalida com string\n", getLinha());
            left = TYPE_ERROR;
        } else if (left == TYPE_FLOAT || right == TYPE_FLOAT) {
            left = TYPE_FLOAT;
        } else {
            left = TYPE_INT;
        }
    }
    return left;
}

int factor(void) {
    const int sync[] = {
        VIRGULA, PARENTESES_DIR, PONTO_VIRGULA, FIM, EF
    };

    if (token.nome_token == ID) {
        int indice = token.atributo;
        if(get_tipo(indice) == 'n'){
            erros++;
            printf("[ERRO] linha %d: variavel %s nao declarada\n", getLinha(), get_nome(indice));
            match(ID, sync, "identificador esperado", 5);
            return TYPE_ERROR;
        } else {
            int tipo_var = tipo_char_para_const(get_tipo(indice));
            match(ID, sync, "identificador esperado", 5);
            return tipo_var;
        }
    }

    if (token.nome_token == INT ||
        token.nome_token == FLOAT ||
        token.nome_token == STRING) {
        int t = tipo_literal_por_token(token.nome_token);
        match(token.nome_token, sync, "literal esperado", 5);
        return t;
    }

    if (token.nome_token == PARENTESES_ESQ) {
        match(PARENTESES_ESQ, sync, "esperado '('", 5);
        int t = expr();
        match(PARENTESES_DIR, sync, "esperado ')'", 5);
        return t;
    }

    if (token.nome_token == OP && token.atributo == MENOS) {
        match(OP, sync, "operador unario '-' esperado", 5);
        int t = factor();
        return t;
    }

    sincronizar("fator invalido", sync, 5);
    return TYPE_ERROR;
}
