#include "functions.h"
int main(void){
    system("clear");
    int x, fim=0;
    printf("Bem-vindo/a ao banco de dados\n\n");
    while(fim != 1){
        printf("----- MENU -----\n1-Criar tabela\n2-Listar todas as tabelas\n3-Inserir linha em uma tabela\n4-Listar dados de uma tabela\n5-Pesquisar valor em uma tabela\n0-Sair\n");
        scanf("%d", &x);
        getchar();
        switch(x){
            case 1:
                criar_tabela();
                system("clear");
                printf("Tabela criada com sucesso!\n");
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
                listar_conteudo();
                break;
            case 5:
                system("clear");
                pesquisar_campo();
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