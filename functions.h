#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
/*struct campo{
	int tipo;
	char nome_campo[100];
}*/
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
	//struct campo Campo;
    char nome[100], campo[100];
    int numero, x, sid = 0, fim1 = 0;
    while(fim1!=1){
    	printf("Insira o nome da tabela\n");
    	scanf("%s", nome);
    	fim1 = checar_nome_tabela(nome);
    	if(fim1 == 0) printf("Essa tabela ja existe\n");
    }
    criar_arquivo(nome);
    printf("Insira o numero de campos\n");
    scanf("%d", &numero);
    printf("Deseja criar uma chave primaria padrão?\n1-Sim  0-Nao\n");
    scanf("%d", &sid);
    if(sid == 1){
    	criar_campo(nome, 2, "id");
    }
    else{
    	char chave[100];
    	printf("Insira o nome da chave primaria (deve ser do tipo int)\n");
    	scanf("%s", chave);
    	criar_campo(nome, 2, chave);
    }
    printf("Insira o tipo e o nome de cada campo\n1-char  2-int  3-float  4-double  5-string\nEx:'1 nome_campo'\n");
    for(int i = 1; i <= numero; i++){
        printf("%do - ", i);
        scanf("%d %s", &x, campo);
        if(x >= 1 && x <= 5){
        	criar_campo(nome, x, campo);
        }
        else{
        	printf("Valor Invalido\n");
        	i--;
        }
    }
}
void inserir_linha(){
	char nome[100];
	int fim = 1;
	while(fim!=0){
    	printf("Insira o nome da tabela\n");
    	scanf("%s", nome);
    	fim = checar_nome_tabela(nome);
    	if(fim == 1) printf("Essa tabela nao existe\n");
    }
}