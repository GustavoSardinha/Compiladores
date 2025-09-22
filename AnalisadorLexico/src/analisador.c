#include <stdio.h>
#include <stdlib.h>
#include "definicoes.h"

int estado = 0;
int partida = 0;
int cont_sim_lido = 0;
char *code;

void analisar_arquivo(char* caminho){
    FILE *arquivo;
    long tamanho;

    arquivo = fopen(caminho, "r");    
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }

    fseek(arquivo, 0, SEEK_END);
    tamanho = ftell(arquivo);
    rewind(arquivo);

    code = (char*) malloc((tamanho + 1) * sizeof(char));
    if (code == NULL) {
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }

    fread(code, sizeof(char), tamanho, arquivo);
    code[tamanho] = '\0';

    fclose(arquivo);
}

int falhar(){
	switch(estado){
		case 0: partida = 9; break;
		case 9: partida = 12; break;
		case 12: partida = 20; break;
		case 20: partida = 25; break;
		case 25:
			printf("Erro encontrado no c�digo\n");
			cont_sim_lido++;
			break;
		default:
			printf("Erro do compilador\n");
			break;
	}
	return partida;
}

Token proximo_token()
{
    Token token;
    char c;

    while(code[cont_sim_lido] != '\0')
    {
        c = code[cont_sim_lido];
        switch (estado) {
            case 0:
                if(c == '<') estado = 1;
                else if(c == '=') estado = 5;
                else if(c == '>') estado = 6;
                else {
                    if(c == ' ' || c == '\n' || c == '\t'){
                        cont_sim_lido++;
                        continue;
                    }
                    if(c == ';'){
                        printf("<;,>\n");
                        cont_sim_lido++;
                        token.nome_token = PONTO_VIRGULA;
                        return token;
                    }
                    if(c == '('){
                        printf("<(,>");
                        cont_sim_lido++;
                        token.nome_token = PARENTESES_ESQ;
                        return token;
                    }
                    if(c == ')'){
                        printf("<),>");
                        cont_sim_lido++;
                        token.nome_token = PARENTESES_DIR;
                        return token;
                    }
                    if(c == '{'){
                        printf("<{,>");
                        cont_sim_lido++;
                        token.nome_token = BLOCO_ESQ;
                        return token;
                    }
                    if(c == '}'){
                        printf("<},>");
                        cont_sim_lido++;
                        token.nome_token = BLOCO_DIR;
                        return token;
                    }
                    estado = falhar();
                }
                break;

            case 1:
                cont_sim_lido++;
                c = code[cont_sim_lido];
                if(c == '=') estado = 2;
                else if(c == '>') estado = 3;
                else estado = 4;
                break;

            case 2: // <=
                cont_sim_lido++;
                printf("<relop, LE>");
                token.nome_token = RELOP;
                token.atributo = LE;
                estado = 0;
                return token;

            case 3: // <>
                cont_sim_lido++;
                printf("<relop, NE>");
                token.nome_token = RELOP;
                token.atributo = NE;
                estado = 0;
                return token;

            case 4: // <
                printf("<relop, LT>");
                token.nome_token = RELOP;
                token.atributo = LT;
                estado = 0;
                return token;

            case 5: // =
                cont_sim_lido++;
                printf("<relop, EQ>");
                token.nome_token = RELOP;
                token.atributo = EQ;
                estado = 0;
                return token;

            case 6:
                cont_sim_lido++;
                c = code[cont_sim_lido];
                if(c == '=') estado = 7;
                else estado = 8;
                break;

            case 7: // >=
                cont_sim_lido++;
                printf("<relop, GE>");
                token.nome_token = RELOP;
                token.atributo = GE;
                estado = 0;
                return token;

            case 8: // >
                printf("<relop, GT>");
                token.nome_token = RELOP;
                token.atributo = GT;
                estado = 0;
                return token;
        }
    }

    token.nome_token = EF;
    return token;
}

