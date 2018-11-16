#include "functions.h"
int main(void){
    system("clear");
    int x, fim=0;
    printf("Bem-vindo(a) ao banco de dados\n");
    while(fim != 1){
        printf("Escolha a opcao desejada:\n1-Criar tabela  2-Listar todas as tabelas  3-Inserir linha em uma tabela\n4-Listar dados de uma tabela  0-Sair\n");
        scanf("%d", &x);
        getchar();
        switch(x){
            case 1:
                criar_tabela();
                system("clear");
                printf("Tabela criada com sucesso\n");
                break;
            case 2:
                system("clear");
                printf("Tabelas disponiveis:\n");
                listar();
                break;
            case 3:
                inserir_linha();
                break;
            case 4:
                system("clear");
                printf("Em breve ;)\n");
                break;
            case 0:
                system("clear");
                fim = 1;
                printf("Até logo!\n");
                break;
            default:
                printf("Valor invalido\n");
                break;
        }
        printf("\n");
    }
    return 0;
}