#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
void listar(){
    DIR *dir;
    struct dirent *lsdir;
    char *extensao = malloc(4*sizeof(char)), *dest;
    int tamanho;
    dir = opendir("./");
    /* print all the files and directories within directory */
    while ( ( lsdir = readdir(dir) ) != NULL )
    {
        extensao = strrchr(lsdir->d_name, '.');
        if(strcmp(extensao, ".txt")==0){
            tamanho = strlen(lsdir->d_name);
            dest = malloc((tamanho-4)*sizeof(char));
            printf("%d\n", tamanho);
            strncpy(dest, lsdir->d_name, tamanho-4);
            printf ("%s\n", dest); 
            free(dest);
        }   
    }
    free(extensao);
    closedir(dir);
}
void criar_tabela(){
    char nome[100];
    int numero, x;
    printf("Insira o nome da tabela");
    scanf("%s", nome);
    printf("Insira o numero de campos");
    scanf("%d", &numero);
    for(int i = 0; i < numero; i++){
        printf("Escolha o tipo do campo.\n1-int.\n2-double.\n");
        scanf("%d", &x);
        switch(x){
            case 1:
                printf("Insira o nome do campo")
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
}
int main(void){
    int x, fim=0;
    printf("Bem-vindo(a) ao banco de dados.\n");
    while(fim != 1){
        printf("Escolha a opcao desejada.\n1-Criar tabela.\n2-Listar todas as tabelas.\n0-Sair.\n");
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