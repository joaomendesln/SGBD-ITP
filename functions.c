#include "functions.h"
int checar_nome_tabela(char nome[100]);
void criar_arquivo(char nome[100]);
void inserir_nome(char nome[100]);
void criar_coluna(coluna Coluna);
void ler_nome_tabela(char nome_arquivo[100]);

void criar_tabela(){
	coluna Coluna;
    //char nome[100], Coluna[100];
    int qtd, x, sid = 0, fim = 0;
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
    scanf("%d", &qtd);
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
        printf("Terá auto-incremento?\n1-Sim  0-Não\n");
        scanf("%d", &Coluna.ai);
        Coluna.not_null = 1;
        Coluna.tipo = 2;
    	criar_coluna(Coluna);
    }
    Coluna.ai = 0;
    //criação dos Colunas da tabela
    printf("Insira o tipo e o nome de cada coluna\n1-char  2-int  3-float  4-double  5-string\nEx:'1 nome_Coluna'\n");
    for(int i = 1; i < qtd; i++){
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
                fclose(arquivo);
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

int ler_tabela(char nome[100]){
    char teste, a;
    int cont = 0;
    char provisorio[100];
    strcpy(provisorio, "./tabelas/");
    strcat(provisorio, nome);
    //criação de arquivo com o nome do parâmetro + ".txt"
    FILE *arquivo = fopen(strcat(provisorio, ".txt"), "r");
    if(arquivo == NULL){
        printf("Erro na abertura do arquivo %s\n", nome);
    }
    else{
        fscanf(arquivo, "%s\n", nome);
        fscanf(arquivo, "%c", &teste);
        a = (char)teste;
        while (a != '\n' && !feof(arquivo)){
            fscanf(arquivo, "%c", &teste);
            a = (char) teste;
            if(a == '|') cont++;
        }
        fclose(arquivo);
        return cont;
    }
}

int contar_linhas(char nome[100]){
    int cont = 0;
    char c, a;
    char provisorio[100];
    strcpy(provisorio, "./tabelas/");
    strcat(provisorio, nome);
    //criação de arquivo com o nome do parâmetro + ".txt"
    FILE *leitura = fopen(strcat(provisorio, ".txt"), "r");
    while(!feof(leitura)){
        fscanf(leitura, "%c", &c);
        a = (char) c;
        if(a == '\n') cont++;
    }
    return cont-2;
}

int verificar_chave(char nome[100], char valor[100]){
    coluna Coluna;
    char c, a;
    char provisorio[100], chave[100];
    strcpy(provisorio, "./tabelas/");
    strcat(provisorio, nome);
    //criação de arquivo com o nome do parâmetro + ".txt"
    FILE *verifica = fopen(strcat(provisorio, ".txt"), "r");
    fscanf(verifica, "%s\n", nome);
    fscanf(verifica, "%c", &c);
    a = (char)c;
    while (a != '\n' && !feof(verifica)){
        fscanf(verifica, "%c", &c);
        a = (char) c;
        if(feof(verifica)){
            fclose(verifica);
            return 0;
        } 
    }
    while(!feof(verifica)){
        fscanf(verifica, "%s | ", chave);
        if(strcmp(chave, valor) == 0) {
            fclose(verifica);
            return 1;
        }
        else{
            while (a != '\n' && !feof(verifica)){
                fscanf(verifica, "%c", &c);
                a = (char) c;
            }
        }
    }
    fclose(verifica);
    return 0;
}

int ultimo_id_tabela(char nome[100]){
    char provisorio[100];
    char c, a;
    int cont = 0, boolean = 1, chave = 0;
    strcpy(provisorio, "./tabelas/");
    strcat(provisorio, nome);
    //criação de arquivo com o nome do parâmetro + ".txt"
    FILE *arquivo = fopen(strcat(provisorio, ".txt"), "a+");
    //caso de erro: arquivo não abre
    if(arquivo == NULL){
        printf("Erro na abertura do arquivo %s\n", nome);
    }
    else {
        while (fscanf(arquivo, "%c", &c) != EOF) {
            a = (char) c;
            if(a == '\n') {
                cont++;
            }
        }
        cont++;
        //printf("%d\n", cont);
        if (cont == 2) return 0;
        else {
            int caracteres[cont];

            for(int i = 0; i < cont; i++){
                caracteres[i] = 0;
            }
            fseek(arquivo, 0, SEEK_SET);
            for(int i = 0; i < cont; i++){
                while (fscanf(arquivo, "%c", &c) != EOF) {
                    caracteres[i] = caracteres[i] + 1;
                    //printf("%c", c);
                    a = (char) c;
                    if(a == '\n') {
                        break;
                    }
                } 
            }

            /*for(int i = 0; i < cont; i++){
                printf("%d ", caracteres[i]);   
            }*/
            //printf("\n");
            fseek(arquivo, (-1 * caracteres[cont - 1]) - 1, SEEK_END);
            fscanf(arquivo, "%d", &chave);
            //printf("%d\n", chave);
            fclose(arquivo);
            return chave;
        }
    }
}

void chamar_campos(char nome[100], int qtd){
    coluna Coluna;
    char provisorio[100], valor[100];
    long long int x;
    int cont = 0, fim = 1;
    strcpy(provisorio, "./tabelas/");
    strcat(provisorio, nome);
    //criação de ponteiros para escrita e leitura com o nome do parâmetro + ".txt"
    FILE *leitura = fopen(strcat(provisorio, ".txt"), "r");
    FILE *escrita = fopen(provisorio, "a");
    //pulando a primeira linha (há apenas o nome da tabela)
    fscanf(leitura, "%s\n", nome);
    //inserindo uma quebra de linha para inserir as próximas informações
    fprintf(escrita, "\n");
    for(int i = 0; i < qtd; i++){
        fscanf(leitura, "%d %d %d %s | ", &Coluna.tipo, &Coluna.ai, &Coluna.not_null, Coluna.nome_coluna);
        if(strcmp(Coluna.nome_coluna, "id") != 0 && i != 0){
            printf("Insira o conteudo para a coluna \"%s\"\n", Coluna.nome_coluna);
            scanf("%s", valor);
            fprintf(escrita, "%s | ", valor);
        }
        else{
            if(Coluna.ai == 1){
                cont = ultimo_id_tabela(nome);
                fprintf(escrita, "%d | ", cont + 1);
            }
            else{
                while(fim!=0){
                    printf("Insira o conteudo para a chave primaria \"%s\"\n", Coluna.nome_coluna);
                    scanf("%lli", &x);
                    sprintf(valor, "%lli", x);
                    fim = verificar_chave(nome, valor);
                    if(fim == 1) printf("Esse valor já foi inserido\n");
                }
                fprintf(escrita, "%s | ", valor);
            }
        }
    }
    //fprintf(escrita, "\n");
    fclose(leitura);
    fclose(escrita);
}
void inserir_linha(){
	char nome[100];
	int fim = 1, colunas;
	while(fim!=0){
    	printf("Insira o nome da tabela\n");
    	scanf("%s", nome);
    	fim = checar_nome_tabela(nome);
    	if(fim == 1) printf("Essa tabela nao existe\n");
    }
    colunas = ler_tabela(nome);
    //printf("%d\n", colunas);
    chamar_campos(nome, colunas);
}

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

void listar_conteudo(){
    char nome[100], a, c;
    int fim = 1;
    while(fim!=0){
        printf("Insira o nome da tabela\n");
        scanf("%s", nome);
        fim = checar_nome_tabela(nome);
        if(fim == 1) printf("Essa tabela nao existe\n");
    }
    printf("\n");
    char provisorio[100];
    strcpy(provisorio, "./tabelas/");
    strcat(provisorio, nome);
    //criação de arquivo com o nome do parâmetro + ".txt"
    FILE *arquivo = fopen(strcat(provisorio, ".txt"), "r");
    //caso de erro: arquivo não abre
    if(arquivo == NULL){
        printf("Erro na abertura do arquivo %s\n", nome);
    }
    else{
        fscanf(arquivo, "%s\n", nome);
        fscanf(arquivo, "%c", &c);
        a = (char)c;
        while (a != '\n' && !feof(arquivo)){
            fscanf(arquivo, "%c", &c);
            a = (char) c;
            if(feof(arquivo)){
                printf("Não há conteudo na tabela escolhida");
            } 
        }
        while (!feof(arquivo)){
            fscanf(arquivo, "%c", &c);
            a = (char) c;
            printf("%c", a);
        }
        fclose(arquivo);
        printf("\n");
    }
}

void pesquisar_registro(char nome[100], int posicao){
    char provisorio[100];
    strcpy(provisorio, "./tabelas/");
    strcat(provisorio, nome);
    //criação de arquivo com o nome do parâmetro + ".txt"
    FILE *arquivo = fopen(strcat(provisorio, ".txt"), "r");
    //caso de erro: arquivo não abre
    if(arquivo == NULL){
        printf("Erro na abertura do arquivo %s\n", nome);
    }
    else{

        fclose(arquivo);
    }
}

void pesquisar_campo(){
    char nome[100], campo[100], a, c;
    int fim = 1, cont = 0, posicao;
    while(fim!=0){
        printf("Insira o nome da tabela\n");
        scanf("%s", nome);
        fim = checar_nome_tabela(nome);
        if(fim == 1) printf("Essa tabela nao existe\n");
    }
    printf("\n");
    char provisorio[100];
    strcpy(provisorio, "./tabelas/");
    strcat(provisorio, nome);
    //criação de arquivo com o nome do parâmetro + ".txt"
    FILE *arquivo = fopen(strcat(provisorio, ".txt"), "r");
    //caso de erro: arquivo não abre
    if(arquivo == NULL){
        printf("Erro na abertura do arquivo %s\n", nome);
    }
    else{
        fscanf(arquivo, "%s\n", nome);
        while (a != '\n' && !feof(arquivo)){
            fscanf(arquivo, "%c", &c);
            a = (char) c;
            if(a == '|') cont++;
            if(feof(arquivo)){
                printf("Não há conteudo na tabela escolhida");
            } 
        }
        coluna *Colunas = NULL;
        Colunas = malloc(cont*sizeof(coluna));
        fseek(arquivo, 0, SEEK_SET);
        fscanf(arquivo, "%s\n", nome);
        for(int i = 0; i < cont; i++){
            fscanf(arquivo, "%d %d %d %s | ", &Colunas[i].tipo, &Colunas[i].ai, &Colunas[i].not_null, Colunas[i].nome_coluna);
        }
        printf("Colunas disponiveis:\n");
        for(int i = 0; i < cont; i++){
            printf("%s  ", Colunas[i].nome_coluna);
            if(i % 3 == 0 && i != 0 && i != (cont-1)) printf("\n");
        }
        fim = 1;
        printf("\n");
        while(fim!=0){
            printf("Insira o campo em que deseja pesquisar\n");
            scanf("%s", campo);
            for(int i = 0; i < cont; i++){
                if(strcmp(campo, Colunas[i].nome_coluna) == 0){
                    fim = 0;
                    posicao = i;
                }
            }
            if(fim == 1) printf("Esse campo não existe\n");
        }
        fclose(arquivo);
        //pesquisar_registro(nome, posicao);
        free(Colunas);
    }
    printf("\n");
}