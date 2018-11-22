#include "functions.h"
#include <stdio.h>
#define TAMANHO 100
FILE* arquivo;

/*void apagar_tabela(){
    int status;
    char *nome;
    nome = malloc(TAMANHO);
    receber_nome_tabela(nome, 1);
    FILE *listaProvisoria = fopen("provisorios/lista_provisoria.txt", "w+");
    FILE *listaTabelas = fopen("tabelas.txt", "r");
    FILE *escreverListaTabelas = fopen("tabelas.txt", "w+");
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
                fprintf(listaProvisoria, "%s\n", nomeTabela);
            }
        }
        fprintf(escreverListaTabelas, "\n");
        fclose(listaTabelas);

        status = remove("tabelas.txt");
        //status += apagar arquivo da tabela
        
        if (status == 0) printf("Tabela deletada com sucesso!");

        fseek(listaProvisoria, 0, SEEK_SET);

        while(fscanf(listaProvisoria, "%s\n", nomeTabela) != EOF){
            printf("%s\n", nomeTabela);
            fprintf(escreverListaTabelas, "%s\n", nomeTabela);
        }
        fclose(escreverListaTabelas);
    }
    free(nome);
    free(nomeTabela);
}*/

void alocar_arquivo(FILE** ptr, char *nome, char modo[]){
    char *provisorio;
    provisorio = malloc(TAMANHO);
    strcpy(provisorio, "./tabelas/");
    strcat(provisorio, nome);
    //criação de arquivo com o nome do parâmetro + ".txt"
    *ptr = fopen(strcat(provisorio, ".txt"), modo);
    free(provisorio);
}

void chamar_campos(char *nome, int qtd){
    coluna Coluna;
    char *valor;
    valor = malloc(TAMANHO);
    int cont = 0, fim = 1, validarCampo = 0;
    //criação de ponteiros para escrita e leitura com o nome do parâmetro + ".txt"
    FILE *leitura, *escrita;
    alocar_arquivo(&leitura, nome, "r");
    alocar_arquivo(&escrita, nome, "a");
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
                        validarCampo = checar_chamada_campo(valor, Coluna.tipo);
                        if (validarCampo == 0) printf("Insira um conteúdo do tipo correto\n");
                    }
                    validarCampo = 0;
                    fim = verificar_chave(nome, valor);
                    if(fim == 1) printf("Esse valor já foi inserido\n");
                }
                fprintf(escrita, "%s | ", valor);
            }
        }
    }
    free(valor);
    fclose(leitura);
    fclose(escrita);
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

    if (pontos > 1) return 0;
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
        int c = checar_char(nome);
        if (c == 0) return 0;
        return 1;
    }
    if (tipo == 2){
        int i = checar_inteiro(nome);
        if (i == 0) return 0;
        return 1;
    }
    if (tipo == 3){
        int f = checar_float(nome);
        if (f == 0) return 0;
        return 1;
    }
    if (tipo == 4){
        int d = checar_float(nome);
        if (d == 0) return 0;
        return 1;
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
    coluna Coluna;
    int qtd, x, sid = 0, fim = 0;
    //estrutura para garantir que a tabela a ser criada ainda não exista
    receber_nome_tabela(Coluna.nome_tabela, 0);
    criar_arquivo(Coluna.nome_tabela);
    inserir_nome(Coluna.nome_tabela);
    while(fim!=1){
        printf("Insira o numero de colunas\n");
        scanf("%d", &qtd);
        if(qtd > 0) fim = 1;
        else printf("Insira um valor maior que zero\n");
    }
    fim = 0;
    while(fim!=1){
        printf("Deseja criar uma chave primaria padrão?\n1-Sim  0-Nao\n");
        scanf("%d", &sid);
        if(sid >= 0 && sid < 2){
            fim = 1;
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
                fim = 0;
                printf("Insira o nome da chave primaria (deve ser do tipo int)\n");
                scanf("%s", Coluna.nome_coluna);
                while(fim!=1){
                    printf("Terá auto-incremento?\n1-Sim  0-Não\n");
                    scanf("%d", &Coluna.ai);
                    if(Coluna.ai >= 0 && Coluna.ai < 2) fim = 1;
                    else printf("Insira um valor válido\n");
                }
                Coluna.not_null = 1;
                Coluna.tipo = 2;
                criar_coluna(Coluna);
            }
        }
        else printf("Insira um valor válido\n");
    }
    fim = 0;   
    Coluna.ai = 0;
    //criação dos Colunas da tabela
    printf("Insira o tipo e o nome de cada coluna\n1-char  2-int  3-float  4-double  5-string\nEx:'1 nome_Coluna'\n");
    for(int i = 2; i <= qtd; i++){
        while(fim != 1){
            printf("%dº - ", i);
            scanf("%d %s", &Coluna.tipo, Coluna.nome_coluna);
            if(Coluna.tipo >= 1 && Coluna.tipo <= 5) fim = 1;
            else printf("Insira um tipo válido\n");
        }
        fim = 0;
        while(fim != 1){
            printf("Aceitará valores nulos?\n1-Nao  0-Sim\n");
            scanf("%d", &Coluna.not_null);
            if(Coluna.not_null >= 0 && Coluna.not_null <= 1) fim = 1;
            else printf("Insira um valor válido\n");
        }
        fim = 0;
        criar_coluna(Coluna);
    }
}

void inserir_linha(){
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
        printf("%d\n", cont);
        fclose(arquivo);
        return cont;
    }
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

void listar_conteudo(){
    char *nome, a, c;
    nome =  malloc(TAMANHO);
    int fim = 1;
    receber_nome_tabela(nome, 1);
    printf("\n");
    alocar_arquivo(&arquivo, nome, "r");
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
    free(nome);
}

void pesquisar_campo(){
    char *nome, *campo, a, c;
    nome = malloc(TAMANHO);
    campo = malloc(TAMANHO);
    int fim = 1, cont = 0, posicao, conteudo = 0;
    receber_nome_tabela(nome, 1);
    printf("\n");
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
            free(Colunas);
        }
        fclose(arquivo);
        pesquisar_registro(nome, posicao);
    }
    free(campo);
    free(nome);
}

void pesquisar_registro(char *nome, int posicao){
    char *valor;
    valor = malloc(TAMANHO);
    int x;
    alocar_arquivo(&arquivo, nome, "r");
    //caso de erro: arquivo não abre
    if(arquivo == NULL){
        printf("Erro na abertura do arquivo %s\n", nome);
    }
    else{
        printf("Insira o valor a ser pesquisado\n");
        scanf("%s", valor);
        system("clear");
        while(x != 0){
            printf("----- PESQUISAR VALOR -----\n");
            printf("Escolha a opção para a pesquisa:\n");
            printf("1-Valores maiores que o valor informado\n");
            printf("2-Valores maiores que ou iguais ao valor informado\n");
            printf("3-Valores iguais ao valor informado\n");
            printf("4-Valores menores que o valor informado\n");
            printf("5-Valores menores que ou iguais ao valor informado\n");
            printf("6-Valores próximo ao valor informado\n");
            printf("0-Parar pesquisa\n");
            scanf("%d", &x);
            switch(x){
                case 1:
                    system("clear");
                    break;
                case 2:
                    system("clear");
                    break;
                case 3:
                    system("clear");
                    fseek(arquivo, 0, SEEK_SET);
                    fscanf(arquivo, "%s\n", nome);
                    fscanf(arquivo, "\n");
                    fscanf(arquivo, "%s", valor);
                    printf("%s\n", valor);
                    break;
                case 4:
                    system("clear");
                    break;
                case 5:
                    system("clear");
                    break;
                case 6:
                    system("clear");
                    break;
                case 0:
                    system("clear");
                    break;
                default:
                    system("clear");
                    printf("Opção inválida\n");
            }
        }
        fclose(arquivo);
    }
    free(valor);
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