#include "functions.h"
int checar_nome_tabela(char nome[100]);
void criar_arquivo(char nome[100]);
void inserir_nome(char nome[100]);
void criar_coluna(coluna Coluna);
void criar_tabela(){
	coluna Coluna;
    //char nome[100], Coluna[100];
    int numero, x, sid = 0, fim = 0;
    //estrutura para garantir que a tabela a ser criada ainda não exista
    while(fim!=1){
    	printf("Insira o nome da tabela\n");
    	scanf("%s", Coluna.nome_tabela);
    	fim = checar_nome_tabela(Coluna.nome_tabela);
    	if(fim == 0) printf("Essa tabela ja existe\n");
    }
    criar_arquivo(Coluna.nome_tabela);
    inserir_nome(Coluna.nome_tabela);
    printf("Insira o numero de colunas\n");
    scanf("%d", &numero);
    printf("Deseja criar uma chave primaria padrão?\n1-Sim  0-Nao\n");
    scanf("%d", &sid);
    //chave primária padrão é criada
    if(sid == 1){
        strcpy(Coluna.nome_coluna, "id");
        Coluna.ai = 1;
        Coluna.not_null = 1;
        Coluna.tipo = 2;
    	criar_coluna(Coluna);
    }
    //chave primária personalizada é criada
    else{
    	printf("Insira o nome da chave primaria (deve ser do tipo int)\n");
    	scanf("%s", Coluna.nome_coluna);
        Coluna.ai = 0;
        Coluna.not_null = 1;
        Coluna.tipo = 2;
    	criar_coluna(Coluna);
    }
    Coluna.ai = 0;
    //criação dos Colunas da tabela
    printf("Insira o tipo e o nome de cada coluna\n1-char  2-int  3-float  4-double  5-string\nEx:'1 nome_Coluna'\n");
    for(int i = 1; i <= numero; i++){
        printf("%do - ", i);
        scanf("%d %s", &Coluna.tipo, Coluna.nome_coluna);
        if(Coluna.tipo >= 1 && Coluna.tipo <= 5){
            printf("Aceitara valores nulos?\n1-Nao  0-Sim\n");
            scanf("%d", &Coluna.not_null);
            if(Coluna.not_null >= 0 && Coluna.not_null <= 1){
                if(Coluna.not_null == 0){
                    criar_coluna(Coluna);
                }
                else{
                    criar_coluna(Coluna);
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
    strcpy(provisorio, "./tabelas/");
    strcat(provisorio, nome);
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
void criar_coluna(coluna Coluna){
    char provisorio[100];
    strcpy(provisorio, "./tabelas/");
    strcat(provisorio, Coluna.nome_tabela);
    //abertura de arquivo da tabela nome[100] para escrita
    FILE *arquivo = fopen(strcat(provisorio, ".txt"), "a");
    //caso de erro: o arquivo não abre
    if(arquivo == NULL){
        printf("Erro na abertura do arquivo %s\n", Coluna.nome_tabela);
    }
    //Coluna com as características do parâmetro é criado na tabela
    else{
        fprintf(arquivo, "%d %d %d %s | ", Coluna.tipo, Coluna.ai, Coluna.not_null, Coluna.nome_coluna);
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