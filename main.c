#include "functions.h"
#include <stdio.h>
int main(void){
    system("clear");
    int x, fim=0, inser = 1;
    printf("Bem-vindo/a ao banco de dados\n\n");
    while(fim != 1){
        printf("----- MENU -----\n1-Criar tabela\n2-Listar todas as tabelas\n3-Inserir registro em uma tabela\n4-Listar dados de uma tabela\n5-Pesquisar valor em uma tabela\n7-Apagar uma tabela\n0-Sair\n");
        scanf("%d", &x);
        getchar();
        switch(x){
            case 1:
                system("clear");
                printf("----- CRIAR TABELA -----\n");
                criar_tabela();
                system("clear");
                printf("Tabela criada com sucesso!\n");
                break;
            case 2:
                system("clear");
                printf("----- LISTAR TABELAS -----\n");
                listar();
                break;
            case 3:
                inser = 1;
                while(inser != 0){
                    system("clear");
                    printf("----- INSERIR REGISTRO -----\n");
                    inserir_linha();
                    system("clear");
                    printf("Registro inserido com sucesso!\n");
                    while(fim != 1){
                        printf("Deseja inserir outro?\n1-Sim 0-Não\n");
                        scanf("%d", &inser);
                        if(inser < 0 || inser > 1){
                            printf("Insira um valor válido\n");
                        }
                        else fim = 1;
                    }
                    fim = 0;
                }
                break;
            case 4:
                system("clear");
                printf("----- LISTAR CONTEUDO -----\n");
                listar_conteudo();
                break;
            case 5:
                system("clear");
                printf("----- PESQUISAR VALOR -----\n");
                pesquisar_campo();
                break;
            case 7:
                system("clear");
                printf("----- DIGITE O NOME DA TABELA -----\n");
                //apagar_tabela();
                break;
            case 0:
                system("clear");
                fim = 1;
                printf("Até logo!\n");
                break;
            default:
                printf("Valor invalido!\n");
                break;
        }
        printf("\n");
    }
    return 0;
}