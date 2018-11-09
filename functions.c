#include "functions.h"
int checar_nome_tabela(char nome[100]);
void criar_arquivo(char nome[100]);
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
    DIR *dir1;
    struct dirent *lsdir1;
    char *extensao1 = malloc(4*sizeof(char));
    //abertura o diretorio que será varrido
    dir1 = opendir("./");
    //caso de erro: diretório não abre
    if(dir1 == NULL){
        printf("Erro na abertura do diretorio\n");
        //retorno 2 para erros
        return 2;
    }
    else{
        //string auxiliar = parametro + ".txt"
        char provisorio[100];
        strcpy(provisorio, nome);
        strcat(provisorio, ".txt");
        //leitura de todos os arquivos do diretório aberto
        while ( ( lsdir1 = readdir(dir1) ) != NULL )
        {
            
            //printf("%s\n", strcat(provisorio, ".txt"));

            //o ponteiro extensao1 pega a extensao do arquivo varrido
            extensao1 = strrchr(lsdir1->d_name, '.');
            //se a extensão é ".txt", os comandos são executados
            if(strcmp(extensao1, ".txt") == 0){
                //se o nome do arquivo varrido for igual ao parâmetro, o retorno é 0 
                if(strcmp(provisorio, lsdir1->d_name) == 0){
                    free(extensao1);
                    closedir(dir1);
                    return 0;
                }
            }  
        }
        //retorno 1 caso não haja nenhuma arquivo com o nome igual ao parâmetro
        return 1;
    }  
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
        fclose(arquivo);
    }
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
        fclose(arquivo);
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

void ler_nome_tabela(char nome_arquivo[100]);
void listar(){
    DIR *dir;
    struct dirent *lsdir;
    //abertura o diretorio que será varrido
    dir = opendir("./");
    //caso de erro: diretório não abre
    if(dir == NULL){
        printf("Erro na abertura do diretorio\n");
    }
    else{
        char *extensao = malloc(4*sizeof(char));
        int y = 0;
        //leitura de todos os arquivos do diretório
        while ( ( lsdir = readdir(dir) ) != NULL )
        {
            //o ponteiro extensao1 pega a extensao do arquivo varrido
            extensao = strrchr(lsdir->d_name, '.');
            //se a extensão é ".txt", os comandos são executados
            if(strcmp(extensao, ".txt") == 0){
                //função para leitura de nome da tabela é executada
                ler_nome_tabela(lsdir->d_name);
                //como há tabelas no diretírio, variável booleana y é verdadeira
                y = 1;
            }   
        }
        closedir(dir);
        //free(extensao);
        //caso não tenha arquivo .txt no diretório, variável y é falsa
        if(y == 0) printf("Nao ha tabelas\n");
    }
}
void ler_nome_tabela(char nome_arquivo[100]){
    char nome[100];
    //abertura de arquivo para leitura com o nome do parâmetro
    FILE *arquivo = fopen(nome_arquivo, "r");
    //caso de erro: arquivo não abre
    if(arquivo == NULL){
        printf("Erro na abertura do arquivo %s\n", nome_arquivo);
    }
    //print da primeira string do arquivo aberto, que é o nome da tabela
    else{
        fscanf(arquivo, "%s\n", nome);
        printf("%s\n", nome);
        fclose(arquivo);
    }
}