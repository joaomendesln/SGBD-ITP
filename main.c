#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "./functions.h"
int main(void){
    int x, fim=0;
    printf("Bem-vindo(a) ao banco de dados\n");
    while(fim != 1){
        printf("Escolha a opcao desejada:\n1-Criar tabela  2-Listar todas as tabelas  0-Sair.\n");
        scanf("%d", &x);
        switch(x){
            case 1:
                criar_tabela();
                break;
            case 2:
                listar();
                break;
            case 0:
                fim = 1;
                break;
            default:
                printf("Valor invalido\n");
                break;
        }
        printf("\n");
    }
    return 0;
}