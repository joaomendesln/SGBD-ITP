#include "functions.h"
#include "checagem.h"
#include "convert.h"
#include <stdio.h>
int main(void){
    limpar();
    char *valor = malloc(100);
    int x, fim=0, inser = 1;
    printf("Bem-vindo/a ao banco de dados\n\n");
    while(fim != 1){
        printf("----- MENU PRINCIPAL -----\n1-Criar tabela\n2-Listar todas as tabelas\n3-Inserir registro em uma tabela\n4-Listar dados de uma tabela\n5-Pesquisar valor em uma tabela\n6-Apagar um registro de uma tabela\n7-Apagar uma tabela\n8-Atualizar registro\n0-Sair\n");
        receber_campo(valor, 2, 1);
        //getchar();
        if(checar_inteiro(valor) == 1){
            x = converter_string_em_inteiro(valor);
            switch(x){
                case 1:
                    limpar();
                    printf("----- CRIAR TABELA -----\n");
                    criar_tabela();
                    break;
                case 2:
                    limpar();
                    printf("----- LISTAR TABELAS -----\n");
                    listar();
                    break;
                case 3:
                    inserir_linha();
                    break;
                case 4:
                    limpar();
                    printf("----- LISTAR CONTEUDO -----\n");
                    listar();
                    printf("\n");
                    escolher_listagem();
                    break;
                case 5:
                    limpar();
                    printf("----- PESQUISAR VALOR -----\n");
                    listar();
                    printf("\n");
                    pesquisar_campo();
                    break;
                case 6:
                    limpar();
                    printf("----- APAGAR REGISTRO -----\n");
                    listar();
                    printf("\n");
                    apagar_registro();
                    break;
                case 7:
                    limpar();
                    printf("----- APAGAR TABELA -----\n");
                    listar();
                    printf("\n");
                    apagar_tabela();
                    break;
                case 8:
                    limpar();
                    printf("----- ATUALIZAR REGISTRO -----\n");
                    listar();
                    printf("\n");
                    atualizar_registro();
                    break;
                case 0:
                    limpar();
                    fim = 1;
                    printf("Até logo!\n");
                    break;
                default:
                    printf("Valor invalido!\n");
                    break;
            }
        }
        else{
            limpar();
            printf("Valor inválido\n");
        }
        printf("\n");
    }
    free(valor);
    return 0;
}