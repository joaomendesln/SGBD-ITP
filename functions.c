#include "functions.h"
int checar_nome_tabela(char nome[100]);
void criar_arquivo(char nome[100]);
void inserir_nome(char nome[100]);
void criar_campo(char nome[100], int cp, int tipo, int ai, int not_null, char campo[100]);
void criar_tabela(){
	struct campo Campo;
    char nome[100], campo[100];
    int numero, x, sid = 0, fim1 = 0;
    //estrutura para garantir que a tabela a ser criada ainda não exista
    while(fim1!=1){
    	printf("Insira o nome da tabela\n");
    	scanf("%s", nome);
    	fim1 = checar_nome_tabela(nome);
    	if(fim1 == 0) printf("Essa tabela ja existe\n");
    }
    criar_arquivo(nome);
    inserir_nome(nome);
    printf("Insira o numero de campos\n");
    scanf("%d", &numero);
    printf("Deseja criar uma chave primaria padrão?\n1-Sim  0-Nao\n");
    scanf("%d", &sid);
    //chave primária padrão é criada
    if(sid == 1){
    	criar_campo(nome, 1, 2, 1, 1, "id");
    }
    //chave primária personalizada é criada
    else{
    	char chave[100];
    	printf("Insira o nome da chave primaria (deve ser do tipo int)\n");
    	scanf("%s", chave);
    	criar_campo(nome, 1, 2, 1, 1, chave);
    }
    //criação dos campos da tabela
    printf("Insira o tipo e o nome de cada campo\n1-char  2-int  3-float  4-double  5-string\nEx:'1 nome_campo'\n");
    for(int i = 1; i <= numero; i++){
        printf("%do - ", i);
        scanf("%d %s", &x, Campo.nome_campo);
        if(x >= 1 && x <= 5){
            printf("Aceitara valores nulos?\n1-Nao  0-Sim\n");
            scanf("%d", &Campo.not_null);
            if(Campo.not_null >= 0 && Campo.not_null <= 1){
                if(Campo.not_null == 0){
                    criar_campo(nome, 0, x, 0, Campo.not_null, Campo.nome_campo);
                }
                else{
                    criar_campo(nome, 0, x, 0, Campo.not_null, Campo.nome_campo);
                }
                
            }
            else{
                printf("Valor Invalido\n");
                i--;
            }
        }
        else{
        	printf("Valor Invalido\n");
        	i--;
        }
    }
}
int checar_nome_tabela(char nome[100]){
    char tabela[100];
    //abertura de arquivo tabelas.txt para leitura 
    FILE *arquivo = fopen("tabelas.txt", "r");
    //caso de erro: o arquivo não abre
    if(arquivo == NULL){
        printf("Erro na abertura do arquivo %s\n", nome);
        //retorno 2 para erros
        return 2;
    }
    //leitura dos nomes das tabelas até o fim do arquivo
    else{
        while (fscanf(arquivo, "%s\n", tabela) != EOF) {
            if (strcmp(nome, tabela) == 0){
                return 0;
            }
        }        
    }
    fclose(arquivo);
    return 1;
}
void criar_arquivo(char nome[100]){
    char provisorio[100];
    strcpy(provisorio, nome);
    //criação de arquivo com o nome do parâmetro + ".txt"
    FILE *arquivo = fopen(strcat(provisorio, ".txt"), "w");
    //caso de erro: arquivo não abre
    if(arquivo == NULL){
        printf("Erro na abertura do arquivo %s\n", nome);
    }
    //nome da tabela é inserido no início do arquivo criado
    else{
        fprintf(arquivo, "%s\n", nome);
    }
    fclose(arquivo);
}
void inserir_nome(char nome[100]){
    //abertura de arquivo tabelas.txt, com listagem do nome de todas as tabelas, para escrita
    FILE *arquivo = fopen("tabelas.txt", "a");
    //caso de erro: arquivo não abre
    if(arquivo == NULL) {
        printf("Erro na abertura do arquivo %s\n", nome);
    }
    //inserir o nome da tabela criada na listagem de tabelas
    else {
        fprintf(arquivo, "%s\n", nome);
    }
    fclose(arquivo);
}
void criar_campo(char nome[100], int cp, int tipo, int ai, int not_null, char campo[100]){
    char provisorio[100];
    strcpy(provisorio, nome);
    //abertura de arquivo da tabela nome[100] para escrita
    FILE *arquivo = fopen(strcat(provisorio, ".txt"), "a");
    //caso de erro: o arquivo não abre
    if(arquivo == NULL){
        printf("Erro na abertura do arquivo %s\n", nome);
    }
    //campo com as características do parâmetro é criado na tabela
    else{
        fprintf(arquivo, "%d %d %d %d %s | ", cp, tipo, ai, not_null, campo);
    }
    fclose(arquivo);
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

//void ler_nome_tabela(char nome_arquivo[100]);
void listar(){
    char nome[100];
    //abertura de arquivo tabelas.txt para leitura 
    FILE *arquivo = fopen("tabelas.txt", "r");
    //caso de erro: o arquivo não abre
    if(arquivo == NULL){
        printf("Erro na abertura do arquivo %s\n", nome);
    }
    //leitura dos nomes das tabelas até o fim do arquivo
    else{
        while (fscanf(arquivo, "%s\n", nome) != EOF){
            printf("%s\n", nome);
        }
    }
    fclose(arquivo);
}