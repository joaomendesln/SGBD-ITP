#include "functions.h"
#include <stdio.h>
#define TAMANHO 100
FILE* arquivo;

void alocar_arquivo(FILE** ptr, char *nome, char modo[]){
    char *provisorio;
    provisorio = malloc(TAMANHO);
    strcpy(provisorio, "./tabelas/");
    strcat(provisorio, nome);
    //criação de arquivo com o nome do parâmetro + ".txt"
    *ptr = fopen(strcat(provisorio, ".txt"), modo);
    free(provisorio);
}

void apagar_registro(){
    int status, fim = 0, validarCampo = 0, qtdColunas = 0, qtdLinhas = 0, escreverLinha = 1, quebrarLinha = 0, tipo = 0, cont = 0;
    char *nome, *valor, *registro, *campo, c, a;
    nome = malloc(TAMANHO);
    valor = malloc(TAMANHO);
    campo = malloc(TAMANHO);
    receber_nome_tabela(nome, 1);
    coluna Coluna;
    FILE *escritaProvisoria = fopen("provisorios/tabela_provisoria.txt", "w+");
    FILE *leituraProvisoria = fopen("provisorios/tabela_provisoria.txt", "r");
    FILE *leitura;
    alocar_arquivo(&leitura, nome, "r");
    while (fscanf(leitura, "%c", &c) != EOF) {
        a = (char) c;
        if(a == '\n') {
            qtdLinhas++;
        }
    }
    fseek(leitura, 0, SEEK_SET);
    if (escritaProvisoria == NULL || leituraProvisoria == NULL || leitura == NULL) printf("Erro na abertura de arquivo");
    else{
        fscanf(leitura, "%s\n", nome);
        fprintf(escritaProvisoria, "%s\n", nome);
        qtdColunas = ler_tabela(nome);
        for (int i = 0; i < qtdColunas; i++){
            fscanf(leitura, "%d %d %d %s | ", &Coluna.tipo, &Coluna.ai, &Coluna.not_null, Coluna.nome_coluna);
            fprintf(escritaProvisoria, "%d %d %d %s | ", Coluna.tipo, Coluna.ai, Coluna.not_null, Coluna.nome_coluna);
            if (i == 0) {
                tipo = Coluna.tipo;
            }
        }

        while(fim!=1){
            printf("Insira a chave primaria do registro que deseja apagar:\n");
            while (validarCampo != 1){
                scanf("%s", valor);
                //cancela(valor);
                validarCampo = checar_chamada_campo(valor, tipo);
                if (validarCampo == 0) printf("Insira um conteúdo do tipo correto\n");
            }
            validarCampo = 0;
            fim = verificar_chave(nome, valor);
            if(fim == 0) printf("Esse valor não existe\n");
        }
        fprintf(escritaProvisoria, "\n");
        while (!feof(leitura)){
            for (int i = 0; i < qtdColunas; i++){
                fscanf(leitura, "%s | ", campo);
                if(i == 0 && strcmp(valor, campo) == 0) {
                    escreverLinha = 0;
                    continue;
                }
                else{
                    if (escreverLinha == 1){
                        fprintf(escritaProvisoria, "%s | ", campo);
                        quebrarLinha = 1;
                    }
                }
            }
            cont++;
            if (quebrarLinha == 1 && cont != qtdLinhas-1) fprintf(escritaProvisoria, "\n");
            escreverLinha = 1;
            quebrarLinha = 0;
            fscanf(leitura, "\n");
        }
        fclose(escritaProvisoria);
        FILE *sobrescreverTabela;
        alocar_arquivo(&sobrescreverTabela, nome, "w+");

        while(fscanf(leituraProvisoria, "%c", &c) != EOF){
            fprintf(sobrescreverTabela, "%c", c);
        }
        fclose(leituraProvisoria);
        fclose(sobrescreverTabela);
    }
    free(campo);
    free(nome);
    free(valor);
}

void apagar_tabela(){
    int status;
    char *nome, *nomeArquivo;
    nome = malloc(TAMANHO);
    nomeArquivo = malloc(TAMANHO + 8);
    receber_nome_tabela(nome, 1);
    FILE *escritaProvisoria = fopen("provisorios/lista_provisoria.txt", "w+");
    FILE *leituraProvisoria = fopen("provisorios/lista_provisoria.txt", "r");
    FILE *listaTabelas = fopen("tabelas.txt", "r");
    char *nomeTabela;
    nomeTabela = malloc(TAMANHO);
    //caso de erro: arquivo não abre
    if(listaTabelas == NULL){
        printf("Erro na abertura do arquivo %s\n", nome);
    }
    //leitura dos nomes das tabelas até o fim do arquivo
    else{   
        while(fscanf(listaTabelas, "%s\n", nomeTabela) != EOF){
            if(strcmp(nome, nomeTabela) != 0) {
                fprintf(escritaProvisoria, "%s\n", nomeTabela);
            }
        }

        fclose(escritaProvisoria);
        fclose(listaTabelas);
        FILE *sobrescreverTabelas = fopen("tabelas.txt", "w+");

        strcpy(nomeArquivo, "./tabelas/");
        strcat(nomeArquivo, nome);
        strcat(nomeArquivo, ".txt");
        status = remove(nomeArquivo);
        limpar();
        if (status == 0) printf("Tabela apagada com sucesso!\n");

        while(fscanf(leituraProvisoria, "%s\n", nomeTabela) != EOF){
            fprintf(sobrescreverTabelas, "%s\n", nomeTabela);
        }
        fclose(leituraProvisoria);
        fclose(sobrescreverTabelas);
    }
    free(nome);
    free(nomeArquivo);
    free(nomeTabela);
}

void chamar_campos(char *nome, int qtd){
    coluna Coluna;
    char *valor;
    valor = malloc(TAMANHO);
    int cont = 0, fim = 1, validarCampo = 0, loop = 1, inser = 1;
    while(loop != 0){
        //criação de ponteiros para escrita e leitura com o nome do parâmetro + ".txt"
        FILE *leitura, *escrita;
        alocar_arquivo(&leitura, nome, "r");
        alocar_arquivo(&escrita, nome, "a");
        fseek(leitura, 0, SEEK_SET);
        fseek(escrita, 0, SEEK_SET);
        //pulando a primeira linha (há apenas o nome da tabela)
        fscanf(leitura, "%s\n", nome);
        //inserindo uma quebra de linha para inserir as próximas informações
        fprintf(escrita, "\n");
        for(int i = 0; i < qtd; i++){
            fscanf(leitura, "%d %d %d %s | ", &Coluna.tipo, &Coluna.ai, &Coluna.not_null, Coluna.nome_coluna);
            if(strcmp(Coluna.nome_coluna, "id") != 0 && i != 0){
                printf("Insira o conteudo para a coluna \"%s\"\n", Coluna.nome_coluna);
                while (validarCampo != 1){
                    scanf("%s", valor);
                    validarCampo = checar_chamada_campo(valor, Coluna.tipo);
                    if (validarCampo == 0) printf("Insira um conteúdo do tipo correto\n");
                }
                validarCampo = 0;
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
                        while (validarCampo != 1){
                            scanf("%s", valor);
                            //cancela(valor);
                            validarCampo = checar_chamada_campo(valor, Coluna.tipo);
                            if (validarCampo == 0) printf("Insira um conteúdo do tipo correto\n");
                        }
                        validarCampo = 0;
                        fim = verificar_chave(nome, valor);
                        if(fim == 1) printf("Esse valor já foi inserido\n");
                    }
                    fim = 1;
                    fprintf(escrita, "%s | ", valor);
                }
            }
        }
        fclose(leitura);
        fclose(escrita);
        limpar();
        printf("----- INSERIR REGISTRO -----\n");
        printf("Registro inserido com sucesso!\n");
        while(inser != 0){
            printf("Deseja inserir outro registro na tabela %s?\n1-Sim 0-Não\n", nome);
            scanf("%d", &loop);
            if(loop < 0 || loop > 1){
                printf("Insira um valor válido");
            }
            else{
                inser = 0;
            }
        }
        inser = 1;
    }
    free(valor);  
}

int checar_char(char *nome){
    int tamanho = strlen(nome);
    if (tamanho > 1) return 0;
    return 1;
}

int checar_float(char *nome){
    int tamanho = strlen(nome), x, p, pontos = 0;

    p = (int)nome[0];
    if (p != 45 && p < 48 || p > 57){
        return 0;
    }

    for (int i = tamanho - 1; i > 0; i--){
        x = (int)nome[i];
        if (x != 46 && x < 48 || x > 57){
            return 0;
        }
    }

    for (int i = tamanho - 1; i > 0; i--){
        x = (int)nome[i];
        if (x == 46) pontos++;
    }

    if (pontos > 1 || pontos == 0) return 0;
    return 1;
}

int checar_inteiro(char *nome){
    int tamanho = strlen(nome), x, p;

    p = (int)nome[0];
    if (p != 45 && p < 48 || p > 57){
        return 0;
    }

    for (int i = tamanho - 1; i > 0; i--){
        x = (int)nome[i];
        if (x < 48 || x > 57){
            return 0;
        }
    }
    return 1;
}

int checar_chamada_campo(char *nome, int tipo){
    if (tipo == 1){
        return checar_char(nome);
    }
    if (tipo == 2){
        return checar_inteiro(nome);
    }
    if (tipo == 3){
        return checar_float(nome);
    }
    if (tipo == 4){
        return checar_float(nome);
    }
    return 1;
}

int checar_nome_tabela(char *nome){
    char *tabela;
    tabela = malloc(TAMANHO);
    //abertura de arquivo tabelas.txt para leitura 
    arquivo = fopen("tabelas.txt", "r");
    //caso de erro: o arquivo não abre
    if(arquivo == NULL){
        printf("Erro na abertura do arquivo tabelas\n");
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
    free(tabela);
    fclose(arquivo);
    //retorno 1 caso não haja uma tabela com o nome escolhido
    return 1;
}

int compara_string_proxima(char *a, char *b){
    int cont = 0, igual = 0;
    while(a[cont] != '\0' && b[cont] != '\0'){
        if(a[cont] == b[cont]){
            igual++;
        }
        cont++;
    }
    if(igual == 0) return 10;
    return (int)(cont/igual);
}

double converter_string_em_double(char *nome){
    int tamanho = strlen(nome), p = (int)nome[0], c, posicao = 0, qtdDecimais = 0;
    double cont = 0, numero = 0, decimal = 0;
    for (int i = 0; i < tamanho; i++){
        c = (int)nome[i];
        if (c == 46) {
            posicao = i;
            break;
        }
    }

    posicao++;
    if(p != 45){
        for (int i = posicao - 2; i >= 0; i--){
            int x = (int)nome[i] % 48;
            numero += x * pow(10,cont);
            cont++;
        }
    }
    else{
        for (int i = posicao - 2; i > 0; i--){
            int x = (int)nome[i] % 48;
            numero += x * pow(10,cont);
            cont++;
        }
        numero *= -1;
    }
    cont = 0;
    for (int i = tamanho - 1; i >= posicao; i--){
        int x = (int)nome[i] % 48;
        decimal += x * pow(10,cont);
        cont++;
    }
    decimal = decimal / pow(10,cont);
    if (p == 45) decimal *= -1;
    return (numero + decimal);
}

int converter_string_em_inteiro(char *nome){
    int tamanho = strlen(nome), numero = 0, sinal = 1, p = (int)nome[0];
    float cont = 0;
    if(p != 45){
        for (int i = tamanho - 1; i >= 0; i--){
            int x = (int)nome[i] % 48;
            numero += x * pow(10,cont);
            cont++;
        }
    }
    else{
        for (int i = tamanho - 1; i > 0; i--){
            int x = (int)nome[i] % 48;
            numero += x * pow(10,cont);
            cont++;
        }
        numero *= -1;
    }
    return numero;
}

void criar_arquivo(char *nome){
    alocar_arquivo(&arquivo, nome, "w");
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

void criar_coluna(coluna Coluna){
    alocar_arquivo(&arquivo, Coluna.nome_tabela, "a");
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

void criar_tabela(){
	coluna *Colunas;
    char *nome = malloc(100);
    int qtd, x, sid = 0, fim = 0;
    //estrutura para garantir que a tabela a ser criada ainda não exista
    receber_nome_tabela(nome, 0);
    while(fim!=1){
        printf("Insira o numero de colunas\n");
        scanf("%d", &qtd);
        if(qtd > 0) fim = 1;
        else printf("Insira um valor maior que zero\n");
    }
    fim = 0;
    Colunas = (coluna *) malloc(qtd*sizeof(coluna));
    for(int i = 0; i < qtd; i++){
        strcpy(Colunas[i].nome_tabela, nome);
    }
    free(nome);
    while(fim!=1){
        printf("Deseja criar uma chave primaria padrão?\n1-Sim  0-Nao\n");
        scanf("%d", &sid);
        if(sid >= 0 && sid < 2){
            fim = 1;
            //chave primária padrão é criada
            if(sid == 1){
                strcpy(Colunas[0].nome_coluna, "id");
                Colunas[0].ai = 1;
                Colunas[0].not_null = 1;
                Colunas[0].tipo = 2;
            }
            //chave primária personalizada é criada
            else{
                fim = 0;
                printf("Insira o nome da chave primaria (deve ser do tipo int)\n");
                scanf("%s", Colunas[0].nome_coluna);
                while(fim!=1){
                    printf("Terá auto-incremento?\n1-Sim  0-Não\n");
                    scanf("%d", &Colunas[0].ai);
                    if(Colunas[0].ai >= 0 && Colunas[0].ai < 2) fim = 1;
                    else printf("Insira um valor válido\n");
                }
                Colunas[0].not_null = 1;
                Colunas[0].tipo = 2;
            }
        }
        else printf("Insira um valor válido\n");
    }
    fim = 0;  
    for(int i = 1; i < qtd; i++){
        Colunas[i].ai = 0;
    } 
    //criação dos Colunas da tabela
    printf("Insira o tipo e o nome de cada coluna\n1-char  2-int  3-float  4-double  5-string\nEx:'1 nome_Coluna'\n");
    for(int i = 2; i <= qtd; i++){
        while(fim != 1){
            printf("%dº - ", i);
            scanf("%d %s", &Colunas[i-1].tipo, Colunas[i-1].nome_coluna);
            if(Colunas[i-1].tipo >= 1 && Colunas[i-1].tipo <= 5) {
                fim = 1;
                for(int j = 0; j < i-1; j++){
                    if(strcmp(Colunas[j].nome_coluna, Colunas[i-1].nome_coluna) == 0){
                        printf("Já existe um campo com esse nome\n");
                        fim = 0;
                    }
                }
            }
            else printf("Insira um tipo válido\n");
        }
        fim = 0;
        while(fim != 1){
            printf("Aceitará valores nulos?\n1-Nao  0-Sim\n");
            scanf("%d", &Colunas[i-1].not_null);
            if(Colunas[i-1].not_null >= 0 && Colunas[i-1].not_null <= 1) fim = 1;
            else printf("Insira um valor válido\n");
        }
        fim = 0;
    }
    criar_arquivo(Colunas[0].nome_tabela);
    inserir_nome(Colunas[0].nome_tabela);
    for(int i = 0; i < qtd; i++){
        criar_coluna(Colunas[i]);
    }
    free(Colunas);
}

void escolher_listagem(){
    char *nome;
    nome =  malloc(TAMANHO);
    receber_nome_tabela(nome, 1);
    printf("\n");
    listar_conteudo(nome);
    free(nome);
}

int espacos_por_coluna(char *nome, int qtdColunas, int posicao){
    int result = 0;
    coluna Coluna;
    char *valor;
    valor = malloc(TAMANHO);
    alocar_arquivo(&arquivo, nome, "r");
    if(arquivo == NULL){
        printf("Erro na abertura do arquivo %s\n", nome);
    }
    else{
        fscanf(arquivo, "%s\n", nome);
        for (int i = 0; i < qtdColunas; i++){
            fscanf(arquivo, "%d %d %d %s | ", &Coluna.tipo, &Coluna.ai, &Coluna.not_null, Coluna.nome_coluna);
            if (i == posicao){
                if (strlen (Coluna.nome_coluna) > result) result = strlen(Coluna.nome_coluna);
            }

        }
        fscanf(arquivo, "\n");
        while(!feof(arquivo)){
            for (int i = 0; i < qtdColunas; i++){
                fscanf(arquivo, "%s | ", valor);
                if (i == posicao){
                    if (strlen (valor) > result) result = strlen(valor);
                }
            }
            fscanf(arquivo, "\n");
        }
        return result;
    }
    free(valor);
    return 0;
}

void inserir_linha(){
    limpar();
    printf("----- INSERIR REGISTRO -----\n");
    listar();
    printf("\n");
    char *nome;
    nome = malloc(TAMANHO);
    int colunas;
    receber_nome_tabela(nome, 1);
    colunas = ler_tabela(nome);
    chamar_campos(nome, colunas);
}

void inserir_nome(char *nome){
    //abertura de arquivo tabelas.txt, com listagem do nome de todas as tabelas, para escrita
    arquivo = fopen("tabelas.txt", "a");
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

int ler_tabela(char *nome){
    char teste, a;
    int cont = 0;
    alocar_arquivo(&arquivo, nome, "r");
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

void limpar(){
    #ifdef LINUX
        system("clear");
    #elif uns
        system("clear");
    #elif Linux
        system("clear");
    #elif linux
        system("clear");
    #elif WIN32
        system("cls");
    #elif Win32
        system("cls");
    #elif win32
        system("cls");
    #else
        fprintf(stderr, "Sistema inválido\n");
    #endif
}

void listar(){
    char *nome = NULL;
    nome = malloc(TAMANHO);
    int cont = 0;
    //abertura de arquivo tabelas.txt para leitura 
    FILE *arquivo = fopen("tabelas.txt", "r");
    //caso de erro: o arquivo não abre
    if(arquivo == NULL){
        printf("Erro na abertura do arquivo %s\n", nome);
    }
    //leitura dos nomes das tabelas até o fim do arquivo
    else{
        while (fscanf(arquivo, "%s\n", nome) != EOF){
            cont++;
        }
        fseek(arquivo, 0, SEEK_SET);
        if(cont != 0){
            printf("Tabelas disponiveis:\n");
            while (fscanf(arquivo, "%s\n", nome) != EOF){
                printf("%s\n", nome);
            }
        }
        else printf("Você ainda não criou nenhuma tabela\n");
    }
    free(nome);
    fclose(arquivo);
}

void listar_conteudo(char *nome){
    char a, c, *valor;
    valor = malloc(TAMANHO);
    FILE *listagem;
    coluna Coluna;
    alocar_arquivo(&listagem, nome, "r");
    int qtdColunas = ler_tabela(nome);
    int espacos[qtdColunas], alinhar = 0;
    //caso de erro: arquivo não abre
    if(listagem == NULL){
        printf("Erro na abertura do arquivo %s\n", nome);
    }
    else{
        fscanf(listagem, "%s\n", nome);
        fscanf(listagem, "%c", &c);
        a = (char)c;
        while (a != '\n' && !feof(listagem)){
            fscanf(listagem, "%c", &c);
            a = (char) c;
        }
        if(feof(listagem)){
            printf("Não há conteudo na tabela escolhida");
        }
        fseek(listagem, 0, SEEK_SET);
        fscanf(listagem, "%s\n", nome);
        for (int i = 0; i < qtdColunas; i++){
            espacos[i] = espacos_por_coluna(nome, qtdColunas, i);
        }
        for (int i = 0; i < qtdColunas; i++){
            if (i == 0) printf(" ");
            fscanf(listagem, "%d %d %d %s | ", &Coluna.tipo, &Coluna.ai, &Coluna.not_null, Coluna.nome_coluna);
            printf("%s ", Coluna.nome_coluna);
            alinhar = espacos[i] - strlen(Coluna.nome_coluna);
            for (int j = 0; j < alinhar; j++) {
                printf(" ");
            }
            printf("| ");
        }
        printf("\n");
        fscanf(listagem, "\n");

        while (!feof(listagem)){
            for (int i = 0; i < qtdColunas; i++){
                if (i == 0) printf(" ");
                fscanf(listagem, "%s | ", valor);
                printf("%s ", valor);
                alinhar = espacos[i] - strlen(valor);
                for (int j = 0; j < alinhar; j++) {
                    printf(" ");
                }
                printf("| ");

            }
            fscanf(listagem, "\n");
            printf("\n");
        }
        fclose(listagem);
        printf("\n");
    }
    free(valor);
}

void pesquisar_campo(){
    char *nome, *campo, a, c;
    nome = malloc(TAMANHO);
    campo = malloc(TAMANHO);
    int fim = 1, cont = 0, loop = 1, inser = 1, posicao, conteudo = 0, tipo;
    receber_nome_tabela(nome, 1);
    printf("\n");
    while(loop != 0){
        alocar_arquivo(&arquivo, nome, "r");
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
                    conteudo = 1;
                } 
            }
            if(conteudo == 0){
                limpar();
                printf("----- PESQUISAR VALOR -----\n");
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
                            tipo = Colunas[i].tipo;
                        }
                    }
                    if(fim == 1) printf("Esse campo não existe\n");
                }
                free(Colunas);
            }
            fclose(arquivo);
            pesquisar_registro(nome, posicao, tipo);
        }
        while(inser != 0){
            printf("\nDeseja realizar outra busca na tabela %s?\n1-Sim 0-Não\n", nome);
            scanf("%d", &loop);
            if(loop < 0 || loop > 1){
                printf("Insira um valor válido");
            }
            else{
                inser = 0;
            }
        }
        inser = 1;
    }
    free(campo);
    free(nome);
}

void pesquisar_registro(char *nome, int posicao, int tipo){
    char *valor;
    valor = malloc(TAMANHO);
    int x = 1, fim = 0, comp;
    while(fim != 1){
        printf("Insira o valor a ser pesquisado\n");
        scanf("%s", valor);
        fim = checar_chamada_campo(valor, tipo);
        if(fim == 0) printf("Insira um valor do tipo correto\n");
    }
    limpar();
    printf("Conteudo da tabela:\n");
    listar_conteudo(nome);
    printf("\n");
    while(x != 0){
        printf("----- PESQUISAR VALOR -----\n");
        printf("Escolha a opção para a pesquisa:\n");
        printf("1-Valores maiores que o valor informado\n");
        printf("2-Valores maiores que ou iguais ao valor informado\n");
        printf("3-Valores iguais ao valor informado\n");
        printf("4-Valores menores que o valor informado\n");
        printf("5-Valores menores que ou iguais ao valor informado\n");
        printf("6-Valores próximos ao valor informado\n");
        printf("0-Parar pesquisa\n");
        scanf("%d", &x);
        if(x < 0 || x > 6){
            limpar();
            printf("Opção inválida\n");
        }
        else if(x != 0){
            limpar();
            realizar_busca(nome, valor, posicao, tipo, x);
        }
    }
    free(valor);
}

void realizar_busca(char *nome, char *valor, int posicao, int tipo, int x){
    int cont = 0;
    double y, numero;
    char *comparador, a, b, c, d;
    FILE *leitura, *mostrar;
    alocar_arquivo(&leitura, nome, "r");
    alocar_arquivo(&mostrar, nome, "r");
    comparador = malloc(TAMANHO);
    if(tipo == 2) y = converter_string_em_inteiro(valor);
    else if(tipo == 3 || tipo == 4) y = converter_string_em_double(valor);
    //pula as duas primeiras linhas do ponteiro leitura
    fscanf(leitura, "%s\n", nome);
    fscanf(leitura, "%c", &b);
    a = (char)b;
    while(a != '\n' && !feof(leitura)){
        fscanf(leitura, "%c", &b);
        a = (char) b;
    }
    //pula as duas primeiras linhas do ponteiro mostrar
    fscanf(mostrar, "%s\n", nome);
    fscanf(mostrar, "%c", &d);
    c = (char)d;
    while(c != '\n' && !feof(mostrar)){
        fscanf(mostrar, "%c", &d);
        c = (char) d;
    }
    printf("----- Resultado -----\n");
    while(!feof(leitura)){
        for(int i = 0; i <= posicao; i++){
            if((tipo == 2 || tipo == 3 || tipo == 4) && i == posicao) fscanf(leitura, "%lf |", &numero);
            else fscanf(leitura, "%s |", comparador);
            if(i == posicao){
                fscanf(mostrar, "%c", &b);
                a = (char) b;
                if(x == 1){
                    if((tipo == 2 || tipo == 3 || tipo == 4) && numero > y){
                        cont++;
                        printf("%c", a);
                    }
                    else if(tipo == 1 && comparador[0] > valor[0]){
                        cont++;
                        printf("%c", a);
                    }
                    else if(tipo == 5 && strcmp(comparador, valor) > 0){
                        cont++;
                        printf("%c", a);
                    }
                    while(a != '\n' && !feof(mostrar)){
                        fscanf(mostrar, "%c", &b);
                        a = (char) b;
                        if((tipo == 2 || tipo == 3 || tipo == 4) && numero > y) printf("%c", a);
                        else if(tipo == 1 && comparador[0] > valor[0]) printf("%c", a);
                        else if(tipo == 5 && strcmp(comparador, valor) > 0) printf("%c", a);
                    }
                }

                else if(x == 2){
                    if((tipo == 2 || tipo == 3 || tipo == 4) && numero >= y){
                        cont++;
                        printf("%c", a);
                    }
                    else if(tipo == 1 && comparador[0] >= valor[0]){
                        cont++;
                        printf("%c", a);
                    }
                    else if(tipo == 5 && strcmp(comparador, valor) >= 0){
                        cont++;
                        printf("%c", a);
                    }
                    while(a != '\n' && !feof(mostrar)){
                        fscanf(mostrar, "%c", &b);
                        a = (char) b;
                        if((tipo == 2 || tipo == 3 || tipo == 4) && numero >= y) printf("%c", a);
                        else if(tipo == 1 && comparador[0] >= valor[0]) printf("%c", a);
                        else if(tipo == 5 && strcmp(comparador, valor) >= 0) printf("%c", a);
                    }
                }
                else if(x == 3){
                    if((tipo == 2 || tipo == 3 || tipo == 4) && numero == y){
                        cont++;
                        printf("%c", a);
                    }
                    else if(tipo == 1 && comparador[0] == valor[0]){
                        cont++;
                        printf("%c", a);
                    }
                    else if(tipo == 5 && strcmp(comparador, valor) == 0){
                        cont++;
                        printf("%c", a);
                    }
                    while(a != '\n' && !feof(mostrar)){
                        fscanf(mostrar, "%c", &b);
                        a = (char) b;
                        if((tipo == 2 || tipo == 3 || tipo == 4) && numero == y) printf("%c", a);
                        else if(tipo == 1 && comparador[0] == valor[0]) printf("%c", a);
                        else if(tipo == 5 && strcmp(comparador, valor) == 0) printf("%c", a);
                    }
                }
                else if(x == 4){
                    if((tipo == 2 || tipo == 3 || tipo == 4) && numero < y){
                        cont++;
                        printf("%c", a);
                    }
                    else if(tipo == 1 && comparador[0] < valor[0]){
                        cont++;
                        printf("%c", a);
                    }
                    else if(tipo == 5 && strcmp(comparador, valor) < 0){
                        cont++;
                        printf("%c", a);
                    }
                    while(a != '\n' && !feof(mostrar)){
                        fscanf(mostrar, "%c", &b);
                        a = (char) b;
                        if((tipo == 2 || tipo == 3 || tipo == 4) && numero < y) printf("%c", a);
                        else if(tipo == 1 && comparador[0] < valor[0]) printf("%c", a);
                        else if(tipo == 5 && strcmp(comparador, valor) < 0) printf("%c", a);
                    }
                }
                else if(x == 5){
                    if((tipo == 2 || tipo == 3 || tipo == 4) && y >= numero){
                        cont++;
                        printf("%c", a);
                    }
                    else if(tipo == 1 && comparador[0] <= valor[0]){
                        cont++;
                        printf("%c", a);
                    }
                    else if(tipo == 5 && strcmp(comparador, valor) <= 0){
                        cont++;
                        printf("%c", a);
                    }
                    while(a != '\n' && !feof(mostrar)){
                        fscanf(mostrar, "%c", &b);
                        a = (char) b;
                        if((tipo == 2 || tipo == 3 || tipo == 4) && numero <= y) printf("%c", a);
                        else if(tipo == 1 && comparador[0] <= valor[0]) printf("%c", a);
                        else if(tipo == 5 && strcmp(comparador, valor) <= 0) printf("%c", a);
                    }
                }
                else if(x == 6 && tipo == 5){
                    if(compara_string_proxima(valor, comparador) < 2 && strcmp(comparador, "") != 0){
                        cont++;
                        printf("%c", a);
                    }
                    while(a != '\n' && !feof(mostrar)){
                        fscanf(mostrar, "%c", &b);
                        a = (char) b;
                        if(compara_string_proxima(valor, comparador) < 2 && strcmp(comparador, "") != 0) printf("%c", a);
                    }
                }
                fscanf(leitura, "%c", &d);
                c = (char) d;
                while(c != '\n' && !feof(leitura)){
                    fscanf(leitura, "%c", &d);
                    c = (char) d;
                }
            }
        }
    }
    if(cont == 0) printf("Nenhum valor encontrado\n");
    printf("\n");
    fclose(leitura);
    fclose(mostrar);
    free(comparador);
}

void receber_nome_tabela(char *nome, int i){
    int fim;
    //i = 0 para caso necessite que não haja a tabela criada, i = 1 para caso necessite que haja a tabela criada
    if(i == 0){
        fim = 0;
        while(fim!=1){
            printf("Insira o nome da tabela\n");
            scanf("%s", nome);
            fim = checar_nome_tabela(nome);
            if(fim == 0) printf("Essa tabela ja existe\n");
        }
    }
    else{
        fim = 1;
        while(fim!=0){
            printf("Insira o nome da tabela\n");
            scanf("%s", nome);
            fim = checar_nome_tabela(nome);
            if(fim == 1) printf("Essa tabela não existe\n");
        }
    }
}

int ultimo_id_tabela(char *nome){
    char c, a;
    int cont = 0, chave = 0;
    alocar_arquivo(&arquivo, nome, "a+");
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
                    a = (char) c;
                    if(a == '\n') {
                        break;
                    }
                } 
            }
            fseek(arquivo, (-1 * caracteres[cont - 1]) - 1, SEEK_END);
            fscanf(arquivo, "%d", &chave);
            fclose(arquivo);
            return chave;
        }
    }
}

int verificar_chave(char *nome, char *valor){
    coluna Coluna;
    char c, a;
    char *chave;
    chave = malloc(TAMANHO);
    alocar_arquivo(&arquivo, nome, "r");
    fscanf(arquivo, "%s\n", nome);
    fscanf(arquivo, "%c", &c);
    a = (char)c;
    while(a != '\n' && !feof(arquivo)){
        fscanf(arquivo, "%c", &c);
        a = (char) c;
        if(feof(arquivo)){
            fclose(arquivo);
            return 0;
        } 
    }
    while(!feof(arquivo)){
        fscanf(arquivo, "%s | ", chave);
        if(strcmp(chave, valor) == 0) {
            fclose(arquivo);
            return 1;
        }
        else{
            while (a != '\n' && !feof(arquivo)){
                fscanf(arquivo, "%c", &c);
                a = (char) c;
            }
        }
    }
    free(chave);
    fclose(arquivo);
    return 0;
}