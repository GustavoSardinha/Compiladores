#include <stdio.h>
#include <stdlib.h>
#include "definicoes.h"
#include "tabela_nomes.h"
#include <ctype.h>

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
    char lexema[64];
    int li = 0;

    while(code[cont_sim_lido] != '\0')
    {
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
                    if(c == ';') { printf("<;,>\n"); cont_sim_lido++; token.nome_token = PONTO_VIRGULA; return token; }
                    if(c == '(') { printf("<(,>\n"); cont_sim_lido++; token.nome_token = PARENTESES_ESQ; return token; }
                    if(c == ')') { printf("<),>\n"); cont_sim_lido++; token.nome_token = PARENTESES_DIR; return token; }
                    if(c == '{') { printf("<{,>\n"); cont_sim_lido++; token.nome_token = BLOCO_ESQ; return token; }
                    if(c == '}') { printf("<},>\n"); cont_sim_lido++; token.nome_token = BLOCO_DIR; return token; }
                    if(c == '=') { printf("<=,>\n"); cont_sim_lido++; token.nome_token = EQ; return token; }
                    estado = falhar();
                }
                break;

            case 1:
                if(isalpha(c) || isdigit(c) || c == '_')
                {
                    if(li < (int)(sizeof(lexema) - 1)) lexema[li++] = c;
                    cont_sim_lido++;
                }
                else
                {
                    estado = 2;
                }
                break;

            case 2:
                lexema[li] = '\0';
                int idx = buscar(lexema);
                if(idx == -1)
                {
                    inserir_identificador(lexema);
                    idx = nomes - 1;
                    printf("<ID, %d>\n", idx);
                    token.nome_token = ID;
                    token.atributo = idx;
                    estado = 0;
                    return token;
                }
                else if(idx == -2)
                {
                    printf("<%s,>\n", lexema);
                    token.nome_token = ID;
                    token.atributo = idx;
                    estado = 0;
                    return token;
                }
                else
                {
                    printf("<ID, %d>\n", idx);
                    token.nome_token = filtrar_palavra_reservada(lexema);
                    token.atributo = idx;
                    estado = 0;
                    return token;
                }
                break;
        }
    }

    token.nome_token = EF;
    return token;
}


