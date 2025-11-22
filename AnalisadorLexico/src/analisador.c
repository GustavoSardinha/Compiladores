#include <stdio.h>
#include <stdlib.h>
#include "definicoes.h"
#include "tabela_nomes.h"
#include <ctype.h>
#include <string.h>
#include "tabela_valores.h"

int estado = 0;
int partida = 0;
int cont_sim_lido = 0;
char *code;
int linha = 0;


int getLinha(){
    return linha;
}

void analisar_arquivo(char* caminho){
    FILE *arquivo;
    long tamanho;

    arquivo = fopen(caminho, "r");    
    if (!arquivo) {
        fprintf(stderr, "Erro ao abrir o arquivo!\n");
        exit(1);
    }

    fseek(arquivo, 0, SEEK_END);
    tamanho = ftell(arquivo);
    rewind(arquivo);

    code = (char*) malloc((tamanho + 2) * sizeof(char)); 
    if (!code) {
        fprintf(stderr, "Erro ao alocar memoria!\n");
        exit(1);
    }

    fread(code, sizeof(char), tamanho, arquivo);
    code[tamanho] = '\0';
    fclose(arquivo);

    int i = tamanho - 1;
    while (i >= 0 && !isalnum((unsigned char)code[i])) {
        i--;
    }

    if (i < 0) return;

    int fim_palavra = i;
    while (fim_palavra < tamanho && isalnum((unsigned char)code[fim_palavra])) {
        fim_palavra++;
    }

    code[fim_palavra] = '&';
    code[fim_palavra + 1] = '\0';
}


int falhar(){
    switch(estado){
        case 0: partida = 3; break;
        case 3: partida = 6; break;
        case 7: partida = 9; break;
        case 8: partida = 9; break;
        case 9: partida = 10; break;
        case 10:
            fprintf(stderr, "Erro no código\n");
            exit(1);
            break;
        default:
            fprintf(stderr, "Erro do compilador\n");
            exit(1);
            break;
    }
    return partida;
}

Token proximo_token()
{
    Token token;
    token.atributo = 0;
    char c;
    char lexema[512];
    int li = 0;

    while(code[cont_sim_lido] != '\0')
    {
        if(c == '\n')
            linha++;
        if(c == '&')
            break;

        c = code[cont_sim_lido];

        switch (estado)
        {
            case 0:
                if(isalpha(c) || c == '_')
                {
                    li = 0;
                    estado = 1;
                }
                else
                {
                    if(c == ' ' || c == '\n' || c == '\t')
                    {
                        cont_sim_lido++;
                        continue;
                    }

                    if(c == '+') {
                        cont_sim_lido++;
                        token.nome_token = OP; 
                        token.atributo = MAIS;
                        return token;
                    }

                    if (c == '-') {
                        if (code[cont_sim_lido+1] == '-') {
                            cont_sim_lido += 2;

                            if (code[cont_sim_lido] == '[' && code[cont_sim_lido+1] == '[') {
                                cont_sim_lido += 2;
                                estado = 11;
                            } else {
                                estado = 10;
                            }
                            break;
                        } else {
                            cont_sim_lido++;
                            token.nome_token = OP;
                            token.atributo = MENOS;
                            return token;
                        }
                    }

                    if(c == '*') {
                        cont_sim_lido++;
                        token.nome_token = OP;
                        token.atributo = PRODUTO;
                        return token;
                    }

                    if(c == '/') {
                        cont_sim_lido++;
                        token.nome_token = OP;
                        token.atributo = DIVISAO;
                        return token;
                    }

                    if(c == ';') {
                        cont_sim_lido++;
                        token.nome_token = PONTO_VIRGULA;
                        return token;
                    }

                    if(c == '(') {
                        cont_sim_lido++;
                        token.nome_token = PARENTESES_ESQ;
                        return token;
                    }

                    if(c == ')') {
                        cont_sim_lido++;
                        token.nome_token = PARENTESES_DIR;
                        return token;
                    }

                    if(c == '{') {
                        cont_sim_lido++;
                        token.nome_token = BLOCO_ESQ;
                        return token;
                    }

                    if(c == '}') {
                        cont_sim_lido++;
                        token.nome_token = BLOCO_DIR;
                        return token;
                    }

                    if(c == '=') {
                        cont_sim_lido++;
                        token.nome_token = EQ;
                        return token;
                    }

                    if(c == ',') { 
                        cont_sim_lido++;
                        token.nome_token = VIRGULA;
                        return token;
                    }

                    estado = falhar();
                }
                break;

            case 1:
                if(isalpha(c) || isdigit(c) || c == '_') {
                    if(li < 511) lexema[li++] = c;
                    cont_sim_lido++;
                }
                else {
                    estado = 2;
                }

                int prox = cont_sim_lido + 1;
                if (code[prox] == '\0') {
                    lexema[li] = '\0';
                    int idx = buscar(lexema);

                    if(idx == -1) {
                        inserir_identificador(lexema);
                        idx = nomes - 1;

                        token.nome_token = ID;
                        token.atributo = idx;
                    }
                    else if(idx == -2) {
                        token.nome_token = filtrar_palavra_reservada(lexema);
                        token.atributo = idx;
                    }
                    else {
                        token.nome_token = filtrar_palavra_reservada(lexema);
                        token.atributo = idx;
                    }

                    estado = -1;
                    return token;
                }
                break;

            case 2:
                lexema[li] = '\0';
                int idx = buscar(lexema);
                if(idx == -1)
                {
                    inserir_identificador(lexema);
                    idx = nomes - 1;
                    token.nome_token = ID;
                    token.atributo = idx;
                    estado = 0;
                    return token;
                }
                else if(idx == -2)
                {
                    token.nome_token = filtrar_palavra_reservada(lexema);
                    token.atributo = 0;
                    estado = 0;
                    return token;
                }
                else
                {
                    token.nome_token = ID;
                    token.atributo = idx;
                    estado = 0;
                    return token;
                }

            case 3:
                if(isdigit(c)){
                    li = 0;
                    estado = 4;
                }
                else {
                    estado = falhar();
                }
                break;

            case 4:
                if(isdigit(c)) {
                    if(li < 511) lexema[li++] = c;
                    cont_sim_lido++;
                }
                else if(isalpha(c) || c == '_'){
                    fprintf(stderr, "Erro léxico na linha %d: identificador não pode começar com dígito\n", getLinha());
                    exit(1);
                }
                else if(c == '.') {
                    if(li < 511) lexema[li++] = c;
                    cont_sim_lido++;
                    estado = 5;
                }
                else {
                    lexema[li] = '\0';

                    ItemValor item;

                    if(strchr(lexema, '.') != NULL) {
                        item.classe = 'f';
                        item.v.valor_float = atof(lexema);
                        int idx = inserir_valor(&item);
                        token.nome_token = FLOAT;
                        token.atributo = idx;
                    } else {
                        item.classe = 'i';
                        item.v.valor_inteiro = atoi(lexema);
                        int idx = inserir_valor(&item);
                        token.nome_token = INT;
                        token.atributo = idx;
                    }

                    estado = 0;
                    return token;
                }
                break;

            case 5:
                if(isdigit(c)) {
                    if(li < 511) lexema[li++] = c;
                    cont_sim_lido++;
                }
                else {
                    lexema[li] = '\0';
                    ItemValor item;
                    item.classe = 'f';
                    item.v.valor_float = atof(lexema);
                    int idx = inserir_valor(&item);
                    token.nome_token = FLOAT;
                    token.atributo = idx;
                    estado = 0;
                    return token;
                }
                break;

            case 6:
                if(c == '"'){
                    li = 0;
                    cont_sim_lido++;
                    estado = 7;
                }
                break;

            case 7:
                c = code[cont_sim_lido];

                if(c == '\0') {
                    fprintf(stderr, "Erro: string não finalizada\n");
                    exit(1);
                }
                else if(c == '"') {
                    lexema[li] = '\0';

                    ItemValor item;
                    item.classe = 's';
                    item.v.valor_string = strdup(lexema);

                    int idx = inserir_valor(&item);
                    token.nome_token = STRING;
                    token.atributo = idx;

                    cont_sim_lido++;
                    estado = 0;
                    return token;
                }
                else if(c == '\\') {
                    cont_sim_lido++;
                    estado = 8;
                }
                else {
                    if(li < 511) lexema[li++] = c;
                    cont_sim_lido++;
                }
                break;

            case 8:
                c = code[cont_sim_lido];
                switch(c) {
                    case 'a': lexema[li++] = '\a'; break;
                    case 'b': lexema[li++] = '\b'; break;
                    case 'f': lexema[li++] = '\f'; break;
                    case 'n': lexema[li++] = '\n'; break;
                    case 'r': lexema[li++] = '\r'; break;
                    case 't': lexema[li++] = '\t'; break;
                    case 'v': lexema[li++] = '\v'; break;
                    case '\\': lexema[li++] = '\\'; break;
                    case '"': lexema[li++] = '"'; break;
                    default:
                        fprintf(stderr, "Erro: sequência de escape inválida \\%c\n", c);
                        exit(1);
                }
                cont_sim_lido++;
                estado = 7;
                break;

            case 10:
                while(code[cont_sim_lido] != '\n' && code[cont_sim_lido] != '\0') {
                    cont_sim_lido++;
                }
                if(code[cont_sim_lido] == '\n') cont_sim_lido++;
                estado = 0;
                break;

            case 11:
            {
                int nivel = 1;
                while(nivel > 0 && code[cont_sim_lido] != '\0') {

                    if(code[cont_sim_lido] == '[' && code[cont_sim_lido+1] == '[') {
                        nivel++;
                        cont_sim_lido += 2;
                    }
                    else if(code[cont_sim_lido] == ']' && code[cont_sim_lido+1] == ']') {
                        nivel--;
                        cont_sim_lido += 2;
                    }
                    else {
                        cont_sim_lido++;
                    }
                }
                estado = 0;
            }
            break;

        }
    }

    token.nome_token = EF;
    return token;
}



