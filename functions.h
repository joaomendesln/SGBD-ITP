#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
void ler_nome_tabela(char nome_arquivo[100]){
	char nome[100];
	FILE *arquivo = fopen(nome_arquivo, "r");
	fscanf(arquivo, "%s\n", nome);
	printf("%s\n", nome);
	fclose(arquivo);
}
int checar_nome_tabela(char nome[100]){
	DIR *dir1;
    struct dirent *lsdir1;
    char *extensao1 = malloc(4*sizeof(char));
    dir1 = opendir("./");
    char provisorio[100];
    strcpy(provisorio, nome);
    strcat(provisorio, ".txt");
    //lê todos os arquivos de texto do diretorio
    while ( ( lsdir1 = readdir(dir1) ) != NULL )
    {
    	//printf("%s\n", strcat(provisorio, ".txt"));
    	extensao1 = strrchr(lsdir1->d_name, '.');
        if(strcmp(extensao1, ".txt")==0){
        	if(strcmp(provisorio, lsdir1->d_name) == 0){
	        	printf("Essa tabela ja existe\n");
	        	free(extensao1);
    			closedir(dir1);
	        	return 0;
	        }
        }  
    }
    return 1;
}
void listar(){
    DIR *dir;
    struct dirent *lsdir;
    char *extensao = malloc(4*sizeof(char));
    dir = opendir("./");
    //lê todos os arquivos de texto do diretorio
    while ( ( lsdir = readdir(dir) ) != NULL )
    {
        extensao = strrchr(lsdir->d_name, '.');
        if(strcmp(extensao, ".txt")==0){
        	ler_nome_tabela(lsdir->d_name);
        }   
    }
    free(extensao);
    closedir(dir);
}
void criar_arquivo(char nome[100]){
	char provisorio[100];
	strcpy(provisorio, nome);
	FILE *arquivo = fopen(strcat(provisorio, ".txt"), "w");
	fprintf(arquivo, "%s\n", nome);
	fclose(arquivo);
}
void criar_campo(char nome[100], int x, char campo[100]){
	char provisorio[100];
	strcpy(provisorio, nome);
	FILE *arquivo = fopen(strcat(provisorio, ".txt"), "a");
	fprintf(arquivo, "%d %s | ", x, campo);
	fclose(arquivo);
}
void criar_tabela(){
    char nome[100], campo[100];
    int numero, x, sid = 0, fim1 = 0;
    while(fim1!=1){
    	printf("Insira o nome da tabela\n");
    	scanf("%s", nome);
    	fim1 = checar_nome_tabela(nome);
    }
    criar_arquivo(nome);
    printf("Insira o numero de campos\n");
    scanf("%d", &numero);
    printf("Deseja criar uma chave primaria padrão?\n1-Sim  0-Nao\n");
    scanf("%d", &sid);
    criar_campo(nome, 1, "id");
    printf("Insira o tipo e o nome de cada campo\n1-int  2-double\nEx:'1 nome_campo'\n");
    for(int i = 1; i <= numero; i++){
        printf("%do - ", i);
        scanf("%d %s", &x, campo);
        if(x >= 1 && x <= 2){
        	criar_campo(nome, x, campo);
        }
        else{
        	printf("Valor Invalido\n");
        	i--;
        }
    }
}