#include "functions.h"
#include <stdio.h>
int main(void){
    system("clear");
    int x, fim=0, inser = 1;
    printf("Bem-vindo/a ao banco de dados\n\n");
    while(fim != 1){
        printf("----- MENU -----\n1-Criar tabela\n2-Listar todas as tabelas\n3-Inserir registro em uma tabela\n4-Listar dados de uma tabela\n5-Pesquisar valor em uma tabela\n6-Apagar um registro de uma tabela\n7-Apagar uma tabela\n0-Sair\n");
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
                inserir_linha();
                break;
            case 4:
                system("clear");
                printf("----- LISTAR CONTEUDO -----\n");
                listar();
                printf("\n");
                listar_conteudo();
                break;
            case 5:
                system("clear");
                printf("----- PESQUISAR VALOR -----\n");
                listar();
                printf("\n");
                pesquisar_campo();
                break;
            case 6:
                system("clear");
                printf("----- APAGAR REGISTRO -----\n");
                listar();
                printf("\n");
                apagar_registro();
                break;
            case 7:
                system("clear");
                printf("----- APAGAR TABELA -----\n");
                listar();
                printf("\n");
                apagar_tabela();
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