#include "functions.h"
#include "checagem.h"
#include "convert.h"
#include <stdio.h>
#define TAMANHO 100

//aloca o ponteiro de arquivo desejado para o arquivo de nome inserido no modo inserido
void alocar_arquivo(FILE** ptr, char *nome, char modo[]){
    char *provisorio = malloc(TAMANHO);
    if(provisorio == NULL){
        fprintf(stderr, "Erro na alocação de memória \n");
    }
    strcpy(provisorio, "./tabelas/");
    strcat(provisorio, nome);
    *ptr = fopen(strcat(provisorio, ".txt"), modo);
    free(provisorio);
}
//apaga um registro
void apagar_registro(){
    coluna Coluna;
    char *nome, *valor, *campo, c, a;
    nome = malloc(TAMANHO);
    valor = malloc(TAMANHO);
    campo = malloc(TAMANHO);
    int fim = 0, validarCampo = 0, qtdColunas = 0, qtdLinhas = 0, escreverLinha = 1, quebrarLinha = 0, cont = 0, conteudo = 0, tipo = 0;
    FILE *escritaProvisoria = fopen("provisorios/tabela_provisoria.txt", "w+");
    FILE *leitura;
    receber_nome_tabela(nome, 1);
    if (strcmp(nome, "/cancelar") == 0) { limpar(); return; }
    alocar_arquivo(&leitura, nome, "r");
    limpar();
    printf("Conteudo da tabela:\n");
    conteudo = listar_conteudo(nome);
    if(conteudo == -1){
        fclose(escritaProvisoria);
        fclose(leitura);
        free(campo);
        free(nome);
        free(valor);
        return;
    }
    else{
        printf("\n");
        if(escritaProvisoria == NULL || leitura == NULL) {
            printf("Erro na abertura de arquivo\n");
            return;
        }
        else{
            while (fscanf(leitura, "%c", &c) != EOF) {
                a = (char) c;
                if(a == '\n') {
                    qtdLinhas++;
                }
            }
            qtdLinhas--;
            fseek(leitura, 0, SEEK_SET);
            fscanf(leitura, "%s\n", nome);
            fprintf(escritaProvisoria, "%s\n", nome);
            qtdColunas = contar_colunas(nome);
            for (int i = 0; i < qtdColunas; i++){
                transcrever_colunas(leitura, escritaProvisoria, &Coluna);
            }
            while(fim!=1){
                printf("Insira a chave primaria do registro que deseja apagar:\n");
                receber_chave_primaria(valor);
                if (strcmp(valor, "/cancelar") == 0) { limpar(); return; }
                fim = verificar_chave(nome, valor);
                if(fim == 0) printf("Esse valor não existe\n");
            }
            if(qtdLinhas >= 2) fprintf(escritaProvisoria, "\n");
            for (int i = 0; i < qtdLinhas; i++){
                for (int j = 0; j < qtdColunas; j++){
                    tipo = tipo_coluna(nome, j);
                    strcpy(campo, escanear_campo(nome, i, j).nome);
                    if(j == 0 && strcmp(valor, campo) == 0) {   escreverLinha = 0;  }
                    else{
                        if (escreverLinha == 1){
                            if (tipo == 5) fprintf(escritaProvisoria, "\"%s\" | ", campo);
                            else fprintf(escritaProvisoria, "%s | ", campo);
                            quebrarLinha = 1;
                        }
                    }
                }
                cont++;
                if (quebrarLinha == 1 && cont < qtdLinhas-1) fprintf(escritaProvisoria, "\n");
                escreverLinha = 1;
                quebrarLinha = 0;

                printf("\n");
            }
            fclose(leitura);
            fclose(escritaProvisoria);
            FILE *leituraProvisoria = fopen("provisorios/tabela_provisoria.txt", "r");
            FILE *sobrescreverTabela;
            alocar_arquivo(&sobrescreverTabela, nome, "w+");
            if(leituraProvisoria == NULL || sobrescreverTabela == NULL){
                printf("Erro na abertura de arquivo\n");
                return;
            }
            while(fscanf(leituraProvisoria, "%c", &c) != EOF){
                fprintf(sobrescreverTabela, "%c", c);
            }
            fclose(leituraProvisoria);
            fclose(sobrescreverTabela);
            limpar();
            printf("Registro apagado com sucesso!\n");
        }
        free(campo);
        free(nome);
        free(valor);
    }
}
//apaga uma tabela
void apagar_tabela(){
    char *nome, *nomeArquivo, *nomeTabela;
    nome = malloc(TAMANHO);
    nomeArquivo = malloc(TAMANHO + 8);
    nomeTabela = malloc(TAMANHO);
    if(nome == NULL || nomeArquivo == NULL || nomeTabela == NULL){
        fprintf(stderr, "Erro na alocação de memória \n");
    }
    int status;
    FILE *escritaProvisoria = fopen("provisorios/lista_provisoria.txt", "w+");
    FILE *listaTabelas = fopen("tabelas.txt", "r");
    receber_nome_tabela(nome, 1);
    if (strcmp(nome, "/cancelar") == 0) { limpar(); return; }
    if(listaTabelas == NULL || escritaProvisoria == NULL){
        fprintf(stderr, "Erro na abertura do arquivo\n");
        return;
    }
    else{
        while(fscanf(listaTabelas, "%s\n", nomeTabela) != EOF){
            if(strcmp(nome, nomeTabela) != 0) {
                fprintf(escritaProvisoria, "%s\n", nomeTabela);
            }
        }
        fclose(escritaProvisoria);
        fclose(listaTabelas);
        FILE *leituraProvisoria = fopen("provisorios/lista_provisoria.txt", "r");
        FILE *sobrescreverTabelas = fopen("tabelas.txt", "w+");
        if(leituraProvisoria == NULL || sobrescreverTabelas == NULL){
            fprintf(stderr, "Erro na abertura do arquivo\n");
            return;
        }
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
//atualiza um registro
void atualizar_registro(){
    coluna *Colunas;
    char *nome, *campo, *valor, a, c;
    nome = malloc(TAMANHO);
    campo = malloc(TAMANHO);
    valor = malloc(TAMANHO);
    int qtdColunas = 0, qtdLinhas = 0, conteudo = 0, fim = 0, posicao, tipo, linha, cont = 0, not_null;
    FILE *escritaProvisoria = fopen("provisorios/tabela_provisoria.txt", "w+");
    FILE *leitura;
    receber_nome_tabela(nome, 1);
    qtdLinhas = contar_linhas(nome);
    qtdColunas = contar_colunas(nome);
    if (strcmp(nome, "/cancelar") == 0) { limpar(); return; }
    alocar_arquivo(&leitura, nome, "r");
    if(leitura == NULL || escritaProvisoria == NULL){
        printf("Erro na abertura de arquivo\n");
        return;
    }
    printf("\n");
    limpar();
    printf("Conteudo da tabela:\n");
    conteudo = listar_conteudo(nome);
    if(conteudo == -1){
        fclose(escritaProvisoria);
        fclose(leitura);
        free(valor);
        free(campo);
        free(nome);
        return;
    }
    else{
        qtdColunas = contar_colunas(nome);
        if(qtdColunas == 1) printf("Não há colunas disponíveis para alteração\n");
        else{
            Colunas = (coluna *) malloc(qtdColunas*sizeof(coluna));
            fseek(leitura, 0, SEEK_SET);
            fscanf(leitura, "%s\n", nome);
            fprintf(escritaProvisoria, "%s\n", nome);
            for(int i = 0; i < qtdColunas; i++){
                transcrever_colunas(leitura, escritaProvisoria, &Colunas[i]);
            }
            while(fim!=1){
                printf("Insira a chave primaria \"%s\" da coluna que deseja alterar\n", Colunas[0].nome_coluna);
                receber_chave_primaria(valor);
                if (strcmp(valor, "/cancelar") == 0) { limpar(); return; }
                fim = verificar_chave(nome, valor);
                if(fim == 0) printf("Não há uma chave primária com esse valor\n");
            }
            fim = 0;
            printf("Registro encontrado:\n");
            linha = listar_registro(nome, valor);
            if(linha == -1) return;
            while(fim!=1){
                printf("\nInsira a coluna que deseja alterar\n");
                scanf("%s", campo);
                getchar();
                if (strcmp(campo, "/cancelar") == 0) { limpar(); return; }
                for(int i = 1; i < qtdColunas; i++){
                    if(strcmp(campo, Colunas[i].nome_coluna) == 0){
                        fim = 1;
                        posicao = i;
                        tipo = Colunas[i].tipo;
                        not_null = Colunas[i].not_null;
                    }
                }
                if(fim == 0) printf("Esse campo não existe ou não pode ser alterado\n");
            }
            fim = 0;
            printf("Insira o novo conteudo para a coluna \"%s\"\n", campo);
            receber_campo(valor, tipo, not_null);
            if (strcmp(valor, "/cancelar") == 0) { limpar(); return; }
            fprintf(escritaProvisoria, "\n");
            for (int i = 0; i < qtdLinhas; i++){
                for (int j = 0; j < qtdColunas; j++){
                    tipo = tipo_coluna(nome, j);
                    strcpy(campo, escanear_campo(nome, i, j).nome);
                    if(i == linha && j == posicao) {

                        if (tipo == 5) fprintf(escritaProvisoria, "\"%s\" | ", valor);
                        else fprintf(escritaProvisoria, "%s | ", valor);
                    }
                    else{
                        if (tipo == 5) fprintf(escritaProvisoria, "\"%s\" | ", campo);
                        else fprintf(escritaProvisoria, "%s | ", campo);
                    }
                }
                if (cont < qtdLinhas-1) fprintf(escritaProvisoria, "\n");
                cont++;

                printf("\n");
            }
            fclose(escritaProvisoria);
            fclose(leitura);
            FILE *leituraProvisoria = fopen("provisorios/tabela_provisoria.txt", "r");
            FILE *sobrescreverTabela;
            alocar_arquivo(&sobrescreverTabela, nome, "w+");
            if(leituraProvisoria == NULL || sobrescreverTabela == NULL){
                printf("Erro na abertura de arquivo\n");
                return;
            }
            while(fscanf(leituraProvisoria, "%c", &c) != EOF){
                fprintf(sobrescreverTabela, "%c", c);
            }
            fclose(leituraProvisoria);
            fclose(sobrescreverTabela);
            limpar();
            printf("Registro alterado com sucesso!\n");
            free(Colunas);
        }
        free(valor);
        free(campo);
        free(nome);
    }
}
//recebe os campos de um registro para inserir em uma tabela
void chamar_campos(char *nome, int qtdColunas){
    char *valor;
    valor = malloc(TAMANHO);
    if(valor == NULL){
        fprintf(stderr, "Erro na alocação de memória \n");
    }
    int cont = 0, fim = 1, validarCampo = 0, loop = 1, inser = 1;
    FILE *leitura, *escrita;
    coluna Coluna;
    while(loop != 0){
        alocar_arquivo(&leitura, nome, "r");
        alocar_arquivo(&escrita, nome, "a");
        if(leitura == NULL || escrita == NULL){
            fprintf(stderr, "Erro na abertura do arquivo %s\n", nome);
        }
        fseek(leitura, 0, SEEK_SET);
        fseek(escrita, 0, SEEK_SET);
        fscanf(leitura, "%s\n", nome);
        fprintf(escrita, "\n");
        for(int i = 0; i < qtdColunas; i++){
            fscanf(leitura, "%d %d %d %s | ", &Coluna.tipo, &Coluna.ai, &Coluna.not_null, Coluna.nome_coluna);
            if(strcmp(Coluna.nome_coluna, "id") != 0 && i != 0){
                printf("\nInsira o conteudo para a coluna \"%s\"\n", Coluna.nome_coluna);
                receber_campo(valor, Coluna.tipo, Coluna.not_null);
                if (strcmp(valor, "/cancelar") == 0) { limpar(); return; }
                if(Coluna.tipo == 5){
                    fprintf(escrita, "\"%s\" | ", valor);
                }
                else{
                    fprintf(escrita, "%s | ", valor);
                }
            }
            else{
                if(Coluna.ai == 1){
                    cont = ultimo_id_tabela(nome);
                    fprintf(escrita, "%d | ", cont + 1);
                }
                else{
                    while(fim!=0){
                        printf("\nInsira o conteudo para a chave primaria \"%s\"\n", Coluna.nome_coluna);
                        receber_chave_primaria(valor);
                        if (strcmp(valor, "/cancelar") == 0) { limpar(); return; }
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
            scanf("%s", valor);
            getchar();
            if (strcmp(valor, "/cancelar") == 0) { limpar(); return; }
            if(checar_inteiro(valor) == 1) loop = converter_string_em_inteiro(valor);

            if(checar_inteiro(valor) == 1 && loop >= 0 && loop <= 1){
                inser = 0;
            }
            else{
                printf("Insira um valor válido");
            }
        }
        inser = 1;
    }
    free(valor);  
}
//usada no item 5, checa se uma string é "proxima" de outra string na tabela (para melhor compreensão, leia o ReadME)
int compara_string_proxima(char *a, char *b){
    int cont = 0, igual = 0, maxigual = 0, ta = strlen(a), tb = strlen(b), maior, menor;
    if(ta > tb){
        maior = ta;
        menor = tb;
    }
    else if(ta == tb){
        maior = tb;
        menor = tb;
    }
    else{
        maior = tb;
        menor = ta;
    }
    char copia[maior], aux, compara[menor];
    if(maior == ta && menor == tb){
        strcpy(copia, a);
        strcpy(compara, b);
    }
    else if(maior == tb && menor == ta){
        strcpy(copia, b);
        strcpy(compara, a);
    }
    else if(maior == tb && menor == tb){
        strcpy(copia, b);
        strcpy(compara, a);
    }
    for(int j = 0; j < maior; j++){
        cont = 0;
        igual = 0;
        for(int i = 0; i < menor; i++){
            if(compara[i] == copia[i]){
                igual++;
            }
            cont++;
        }
        if(igual > maxigual) maxigual = igual;
        aux = copia[0];
        for(int k = 0; k < maior; k++){
            if(k == maior-1){
                copia[k] = aux;
            }
            else{
                copia[k] = copia[k+1];
            }
        }
    }
    if(maxigual == 0) return 10;
    return (int)(cont/maxigual);
}
//conta quantas colunas uma tabela tem
int contar_colunas(char *nome){
    char teste, a;
    int cont = 0;
    FILE *arquivo;
    alocar_arquivo(&arquivo, nome, "r");
    if(arquivo == NULL){
        fprintf(stderr, "Erro na abertura do arquivo %s\n", nome);
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
//conta numero de registros
int contar_linhas(char *nome){
    char a, c;
    int qtdLinhas = 0;
    FILE *leitura;
    alocar_arquivo(&leitura, nome, "r");
    if(leitura == NULL){
        fprintf(stderr, "Erro na abertura do arquivo %s\n", nome);
    }
    while (fscanf(leitura, "%c", &c) != EOF) {
        a = (char) c;
        if(a == '\n') {
            qtdLinhas++;
        }
    }
    fclose(leitura);
    return qtdLinhas-1;
}
//cria o arquivo
void criar_arquivo(char *nome){
    FILE *arquivo;
    alocar_arquivo(&arquivo, nome, "w");
    if(arquivo == NULL){
        fprintf(stderr, "Erro na abertura do arquivo %s\n", nome);
    }
    else{
        fprintf(arquivo, "%s\n", nome);
    }
    fclose(arquivo);
}
//insere as colunas e suas especificações no arquivo
void criar_coluna(coluna Coluna){
    FILE *arquivo;
    alocar_arquivo(&arquivo, Coluna.nome_tabela, "a");
    if(arquivo == NULL){
        fprintf(stderr, "Erro na abertura do arquivo %s\n", Coluna.nome_tabela);
    }
    else{
        fprintf(arquivo, "%d %d %d %s | ", Coluna.tipo, Coluna.ai, Coluna.not_null, Coluna.nome_coluna);
    }
    fclose(arquivo);
}
//cria a tabela
void criar_tabela(){
	coluna *Colunas;
    char *nome = malloc(TAMANHO), *lixo = malloc(TAMANHO);
    if(nome == NULL || lixo == NULL){
        fprintf(stderr, "Erro na alocação de memória \n");
    }
    int qtd, x, sid = 0, fim = 0;
    receber_nome_tabela(nome, 0);
    if (strcmp(nome, "/cancelar") == 0) { limpar(); return; }
    while(fim!=1){
        printf("\nInsira o numero de colunas\n");
        receber_campo(lixo, 2, 1);
        if (strcmp(lixo, "/cancelar") == 0) { limpar(); return; }
        qtd = converter_string_em_inteiro(lixo);
        if(qtd > 0) fim = 1;
        else printf("Insira um valor maior que zero\n");
    }
    fim = 0;
    Colunas = (coluna *) malloc(qtd*sizeof(coluna));
    if(Colunas == NULL){
        fprintf(stderr, "Erro na alocação de memória \n");
    }
    for(int i = 0; i < qtd; i++){
        strcpy(Colunas[i].nome_tabela, nome);
    }
    free(nome);
    while(fim!=1){
        printf("\nDeseja criar uma chave primaria padrão?\n1-Sim  0-Nao\n");
        receber_campo(lixo, 2, 1);
        if (strcmp(lixo, "/cancelar") == 0) { limpar(); return; }
        sid = converter_string_em_inteiro(lixo);
        if(sid >= 0 && sid < 2){
            fim = 1;
            if(sid == 1){
                strcpy(Colunas[0].nome_coluna, "id");
                Colunas[0].ai = 1;
                Colunas[0].not_null = 1;
                Colunas[0].tipo = 2;
            }
            else{
                fim = 0;
                printf("Insira o nome da chave primaria (deve ser do tipo int)\n");
                scanf("%s", Colunas[0].nome_coluna);
                getchar();
                if (strcmp(Colunas[0].nome_coluna, "/cancelar") == 0) { limpar(); return; }
                while(fim!=1){
                    printf("\nTerá auto-incremento?\n1-Sim  0-Não\n");
                    receber_campo(lixo, 2, 1);
                    if (strcmp(lixo, "/cancelar") == 0) { limpar(); return; }
                    Colunas[0].ai = converter_string_em_inteiro(lixo);
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
    printf("Insira o tipo e o nome de cada coluna\n1-char  2-int  3-float  4-double  5-string\nEx:'1 nome_Coluna'\n");
    for(int i = 2; i <= qtd; i++){
        while(fim != 1){
            printf("\n%dº - ", i);
            scanf("%s", lixo);
            if (strcmp(lixo, "/cancelar") == 0) { limpar(); return; }
            scanf("%s", Colunas[i-1].nome_coluna);
            getchar();
            if (strcmp(Colunas[i-1].nome_coluna, "/cancelar") == 0) { limpar(); return; }
            if(checar_inteiro(lixo) == 1) Colunas[i-1].tipo = converter_string_em_inteiro(lixo);
            if(checar_inteiro(lixo) == 1 && Colunas[i-1].tipo >= 1 && Colunas[i-1].tipo <= 5) {
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
            printf("\nAceitará valores nulos?\n1-Sim  0-Não\n");
            scanf("%s", lixo);
            getchar();
            if (strcmp(lixo, "/cancelar") == 0) { limpar(); return; }
            if(checar_inteiro(lixo) == 1) Colunas[i-1].not_null = converter_string_em_inteiro(lixo);
            if(checar_inteiro(lixo) == 1 && Colunas[i-1].not_null >= 0 && Colunas[i-1].not_null <= 1) fim = 1;
            else printf("Insira um valor válido\n");
            if(Colunas[i-1].not_null == 0) Colunas[i-1].not_null = 1;
            else Colunas[i-1].not_null = 0;
        }
        fim = 0;
    }
    criar_arquivo(Colunas[0].nome_tabela);
    inserir_nome(Colunas[0].nome_tabela);
    for(int i = 0; i < qtd; i++){
        criar_coluna(Colunas[i]);
    }
    free(Colunas);
    free(lixo);
    limpar();
    printf("Tabela criada com sucesso!\n");
}
//escaneia o campo
campo escanear_campo(char *nome, int linha, int coluna){
    char tabela[100], id[100], c, a = ' ';
    int x = 0, cont = 0, qtdLinhas = contar_linhas(nome), qtdColunas = contar_colunas(nome);
    campo Campo;
    FILE *escanear;
    alocar_arquivo(&escanear, nome, "r");
    if(escanear == NULL){
        fprintf(stderr, "Erro na abertura do arquivo %s\n", nome);
    }
    int fim = 0;

    fscanf(escanear, "%s\n", tabela);

    while(a != '\n'){
        fscanf(escanear, "%c", &c);
        a = (char)c;
    }
    fscanf(escanear, "\n");
    a = ' ';
    while(!feof(escanear)){
        for(int i = 0; i < qtdLinhas; i++){
            if (i == linha){
                for (int j = 0; j < qtdColunas; j++){
                    a = ' ';
                    if (j == coluna){           
                        fscanf(escanear, "%c", &c);
                        a = (char)c;
                        x = (int)c;
                        if (x != 34) {
                            fseek(escanear, -1, SEEK_CUR);
                            fscanf(escanear, "%s | ", Campo.nome);
                        }
                        else{
                            x = 0;
                            while (x != 34){
                                fscanf(escanear, "%c", &c);
                                a = (char)c;
                                x = (int)c;
                                if (x != 34) cont++;
                            }
                            fseek(escanear, -cont - 1, SEEK_CUR);
                            fgets(Campo.nome, cont + 1, escanear);
                            fscanf(escanear, "\" | ");
                            cont = 0;
                        }
                        fscanf(escanear, " | ");
                    }
                    else {
                        while((a != '|' ) && !feof(escanear)){
                            fscanf(escanear, "%c", &c);
                            a = (char)c;
                        }
                        fscanf(escanear, " ");
                    }
                }
            }
            else {
                a = ' ';
                while(a != '\n' && !feof(escanear)){
                    fscanf(escanear, "%c", &c);
                    a = (char)c;
                }
                fscanf(escanear, "\n");
            }
        }
    }
    fclose(escanear);
    return Campo;
}
//escreve o cabeçalho da tabela
void escrever_cabecalho(char *nome){
    char a, c, *valor;
    valor = malloc(TAMANHO);
    if(valor == NULL){
        fprintf(stderr, "Erro na alocação de memória \n");
    }
    FILE *listagem;
    coluna Coluna;
    alocar_arquivo(&listagem, nome, "r");
    int qtdColunas = contar_colunas(nome);
    int espacos[qtdColunas], alinhar = 0, hifens = 0, idRegistro = 0, validar = 0, cont = 0, linha;
    if(listagem == NULL){
        fprintf(stderr, "Erro na abertura do arquivo %s\n", nome);
    }
    else{
        fseek(listagem, 0, SEEK_SET);
        fscanf(listagem, "%s\n", nome);
        for (int i = 0; i < qtdColunas; i++){
            espacos[i] = 0;
        }
        for (int i = 0; i < qtdColunas; i++){
            fscanf(listagem, "%d %d %d %s | ", &Coluna.tipo, &Coluna.ai, &Coluna.not_null, Coluna.nome_coluna);
            if (strlen(Coluna.nome_coluna) > espacos[i]) espacos[i] = strlen(Coluna.nome_coluna); 
        }
        fscanf(listagem, "\n");

        for (int i = 0; i < qtdColunas; i++){
            espacos[i] = espacos_por_coluna(nome, qtdColunas, i);
        }
        fseek(listagem, 0, SEEK_SET);
        fscanf(listagem, "%s\n", nome);
        listar_estilo_linha(qtdColunas, espacos);
        for (int i = 0; i < qtdColunas; i++){
            if (i == 0) printf("| ");
            fscanf(listagem, "%d %d %d %s | ", &Coluna.tipo, &Coluna.ai, &Coluna.not_null, Coluna.nome_coluna);
            printf("%s ", Coluna.nome_coluna);
            alinhar = espacos[i] - strlen(Coluna.nome_coluna);
            for (int j = 0; j < alinhar; j++) {
                printf(" ");
            }
            printf("| ");
        }
        printf("\n");
        listar_estilo_linha(qtdColunas, espacos);       
        fclose(listagem);
    }
    free(valor);
}
//escolhe o arquivo a ter seus dados listados e os lista
void escolher_listagem(){
    char *nome;
    nome =  malloc(TAMANHO);
    if(nome == NULL){
        fprintf(stderr, "Erro na alocação de memória \n");
    }
    receber_nome_tabela(nome, 1);
    if (strcmp(nome, "/cancelar") == 0) { limpar(); return; }
    limpar();
    printf("Conteúdo da tabela:\n");
    listar_conteudo(nome);
    free(nome);
}
//conta o tamanho de caracteres do maior valor em uma coluna(seja o nome da coluna ou um registro)
int espacos_por_coluna(char *nome, int qtdColunas, int posicao){
    char *valor;
    valor = malloc(TAMANHO);
    if(valor == NULL){
        fprintf(stderr, "Erro na alocação de memória \n");
    }
    int result = 0, qtdLinhas = contar_linhas(nome);
    FILE *arquivo;
    coluna Coluna;
    alocar_arquivo(&arquivo, nome, "r");
    if(arquivo == NULL){
        fprintf(stderr, "Erro na abertura do arquivo %s\n", nome);
    }
    else{
        fscanf(arquivo, "%s\n", nome);
        for (int i = 0; i < qtdColunas; i++){
            fscanf(arquivo, "%d %d %d %s | ", &Coluna.tipo, &Coluna.ai, &Coluna.not_null, Coluna.nome_coluna);
            if (i == posicao){
                if (strlen (Coluna.nome_coluna) > result) result = strlen(Coluna.nome_coluna);
            }

        };
        fscanf(arquivo, "\n");
        for (int i = 0; i < qtdLinhas; i++){
            for (int j = 0; j < qtdColunas; j++){
                strcpy(valor, escanear_campo(nome, i, j).nome);
                if (j == posicao){
                    if(strcmp(valor, "null") != 0 && strlen(valor) > result) result = strlen(valor);
                }
            }
        }
        free(valor);
        fclose(arquivo);
        return result;
    }
    fclose(arquivo);
    free(valor);
    return 0;
}
//insere um registro numa tabela
void inserir_linha(){
    limpar();
    printf("----- INSERIR REGISTRO -----\n");
    listar();
    printf("\n");
    char *nome;
    nome = malloc(TAMANHO);
    if(nome == NULL){
        fprintf(stderr, "Erro na alocação de memória \n");
    }
    int colunas;
    receber_nome_tabela(nome, 1);
    if (strcmp(nome, "/cancelar") == 0) { limpar(); return; }
    colunas = contar_colunas(nome);
    chamar_campos(nome, colunas);
    free(nome);
}
//insere o nome de uma nova tabela na lista de tabelas
void inserir_nome(char *nome){
    FILE *arquivo;
    arquivo = fopen("tabelas.txt", "a");
    if(arquivo == NULL) {
        fprintf(stderr, "Erro na abertura do arquivo %s\n", nome);
    }
    else {
        fprintf(arquivo, "%s\n", nome);
    }
    fclose(arquivo);
}
//limpa a tela
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
//lista todas as tabelas
void listar(){
    char *nome = NULL;
    nome = malloc(TAMANHO);
    if(nome == NULL){
        fprintf(stderr, "Erro na alocação de memória \n");
    }
    int cont = 0;
    FILE *arquivo = fopen("tabelas.txt", "r");
    if(arquivo == NULL){
        fprintf(stderr, "Erro na abertura do arquivo %s\n", nome);
    }
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
//lista todo os registros de uma taela e retorna o numero de linhas
int listar_conteudo(char *nome){
    char a, c, *valor;
    valor = malloc(TAMANHO);
    if(valor == NULL){
        fprintf(stderr, "Erro na alocação de memória \n");
    }
    FILE *listagem;
    coluna Coluna;
    alocar_arquivo(&listagem, nome, "r");
    int qtdColunas = contar_colunas(nome), qtdLinhas = contar_linhas(nome);
    int espacos[qtdColunas], alinhar = 0, hifens = 0;
    if(listagem == NULL){
        fprintf(stderr, "Erro na abertura do arquivo %s\n", nome);
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
            printf("Não há conteudo na tabela escolhida\n");
            return -1;
        }
        fseek(listagem, 0, SEEK_SET);
        fscanf(listagem, "%s\n", nome);
        for (int i = 0; i < qtdColunas; i++){
            espacos[i] = espacos_por_coluna(nome, qtdColunas, i);
        }
        listar_estilo_linha(qtdColunas, espacos);
        for (int i = 0; i < qtdColunas; i++){
            if (i == 0) printf("| ");
            fscanf(listagem, "%d %d %d %s | ", &Coluna.tipo, &Coluna.ai, &Coluna.not_null, Coluna.nome_coluna);
            printf("%s ", Coluna.nome_coluna);
            alinhar = espacos[i] - strlen(Coluna.nome_coluna);
            for (int j = 0; j < alinhar; j++) {
                printf(" ");
            }
            printf("| ");
        }
        printf("\n");
        listar_estilo_linha(qtdColunas, espacos);
        fscanf(listagem, "\n");

        for (int i = 0; i < qtdLinhas; i++){
            for (int j = 0; j < qtdColunas; j++){
                strcpy(valor, escanear_campo(nome, i, j).nome);
                if (j == 0) printf("| ");
                if (strcmp(valor, "null") == 0) {
                    printf(" ");
                    alinhar = espacos[j];
                }
                else{ 
                    printf("%s ", valor);
                    alinhar = espacos[j] - strlen(valor);
                }
                for (int k = 0; k < alinhar; k++) {
                    printf(" ");
                }
                printf("| ");
            }
            printf("\n");
        }
        listar_estilo_linha(qtdColunas, espacos);
        fclose(listagem);
    }
    free(valor);
    return 0;
}
//lista um único registro de uma tabela e retorna a posição da sua linha
int listar_registro(char *nome, char *id){
    char a, c, *valor;
    valor = malloc(TAMANHO);
    FILE *listagem;
    coluna Coluna;
    alocar_arquivo(&listagem, nome, "r");
    int qtdColunas = contar_colunas(nome), qtdLinhas = contar_linhas(nome);
    int espacos[qtdColunas], alinhar = 0, hifens = 0, idRegistro = 0, validar = 0, cont = 0, linha;
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
            printf("Não há conteudo na tabela escolhida\n");
            return -1;
        }
        fseek(listagem, 0, SEEK_SET);
        fscanf(listagem, "%s\n", nome);
        for (int i = 0; i < qtdColunas; i++){
            espacos[i] = 0;
        }
        for (int i = 0; i < qtdColunas; i++){
            fscanf(listagem, "%d %d %d %s | ", &Coluna.tipo, &Coluna.ai, &Coluna.not_null, Coluna.nome_coluna);
            if (strlen(Coluna.nome_coluna) > espacos[i]) espacos[i] = strlen(Coluna.nome_coluna); 
        }
        fscanf(listagem, "\n");

        for (int i = 0; i < qtdLinhas; i++){
            for (int j = 0; j < qtdColunas; j++){
                strcpy(valor, escanear_campo(nome, i, j).nome);
                if(j == 0 && strcmp(valor, id) == 0) validar = 1;
                else{
                    if (j == 0 && strcmp(valor,id) != 0) {
                        validar = 0;
                        break;
                    }
                }
                if(validar == 1){
                    if (strlen(valor) > espacos[j]) espacos[j] = strlen(valor);
                }
            }
            fscanf(listagem, "\n");
        }
        fseek(listagem, 0, SEEK_SET);
        fscanf(listagem, "%s\n", nome);
        listar_estilo_linha(qtdColunas, espacos);
        for (int i = 0; i < qtdColunas; i++){
            if (i == 0) printf("| ");
            fscanf(listagem, "%d %d %d %s | ", &Coluna.tipo, &Coluna.ai, &Coluna.not_null, Coluna.nome_coluna);
            printf("%s ", Coluna.nome_coluna);
            alinhar = espacos[i] - strlen(Coluna.nome_coluna);
            for (int j = 0; j < alinhar; j++) {
                printf(" ");
            }
            printf("| ");
        }
        printf("\n");
        listar_estilo_linha(qtdColunas, espacos);
        validar = 0;
        fscanf(listagem, "\n");
        for (int i = 0; i < qtdLinhas; i++){
            for (int j = 0; j < qtdColunas; j++){
                strcpy(valor, escanear_campo(nome, i, j).nome);
                if(j == 0 && strcmp(valor, id) == 0) {
                    validar = 1;
                    linha = cont;
                }
                else{
                    if (j == 0 && strcmp(valor,id) != 0) {
                        validar = 0;
                        break;
                    }
                }
                if (validar == 1){               
                    if (j == 0) printf("| ");
                    if (strcmp(valor, "null") == 0) {
                        printf(" ");
                        alinhar = espacos[j];
                    }
                    else{ 
                        printf("%s ", valor);
                        alinhar = espacos[j] - strlen(valor);
                    }
                    for (int k = 0; k < alinhar; k++) {
                        printf(" ");
                    }
                    printf("| ");
                }
            }
            cont++;
            if (validar == 1) printf("\n");
        }
        listar_estilo_linha(qtdColunas, espacos);        
        fclose(listagem);
    }
    free(valor);
    return linha;
}
//escreve um design, personalizado pelo numero de colunas e o espaço entre cada uma, na tela
void listar_estilo_linha(int qtdColunas, int *espacos){
    printf("+");
    for (int i = 0, j = 0; j < qtdColunas; i++)
    {
        if(espacos[j] + 2 == i) {
            printf("+");
            j++;
            i = -1;
        }
        else {
            printf("-");
        }
    }
    printf("\n");
}
//recebe o campo a ser pesquisado
void pesquisar_campo(){
    char *nome, *campo, a, c;
    nome = malloc(TAMANHO);
    campo = malloc(TAMANHO);
    if(nome == NULL || campo == NULL){
        fprintf(stderr, "Erro na alocação de memória \n");
    }
    int fim = 1, cont = 0, loop = 1, inser = 1, posicao, conteudo = 0, tipo, not_null;
    FILE *arquivo;
    receber_nome_tabela(nome, 1);
    if (strcmp(nome, "/cancelar") == 0) { limpar(); return; }
    printf("\n");
    while(loop != 0){
        alocar_arquivo(&arquivo, nome, "r");
        if(arquivo == NULL){
            fprintf(stderr, "Erro na abertura do arquivo %s\n", nome);
            loop = 0;
        }
        else{
            fscanf(arquivo, "%s\n", nome);
            while (a != '\n' && !feof(arquivo)){
                fscanf(arquivo, "%c", &c);
                a = (char) c;
            }
            if(feof(arquivo)){
                printf("Não há conteudo na tabela escolhida\n");
                conteudo = 1;
            }
            cont = contar_colunas(nome);
            if(conteudo == 0){
                limpar();
                printf("----- PESQUISAR VALOR -----\n");
                coluna *Colunas = NULL;
                Colunas = malloc(cont*sizeof(coluna));
                if(Colunas == NULL){
                    fprintf(stderr, "Erro na alocação de memória \n");
                }
                fseek(arquivo, 0, SEEK_SET);
                fscanf(arquivo, "%s\n", nome);
                for(int i = 0; i < cont; i++){
                    fscanf(arquivo, "%d %d %d %s | ", &Colunas[i].tipo, &Colunas[i].ai, &Colunas[i].not_null, Colunas[i].nome_coluna);
                }
                printf("Colunas disponiveis:\n");
                for(int i = 0; i < cont; i++){
                    printf("%s  ", Colunas[i].nome_coluna);
                }
                fim = 1;
                printf("\n");
                while(fim!=0){
                    printf("Insira o campo em que deseja pesquisar\n");
                    scanf("%s", campo);
                    getchar();
                    if (strcmp(campo, "/cancelar") == 0) { limpar(); return; }
                    for(int i = 0; i < cont; i++){
                        if(strcmp(campo, Colunas[i].nome_coluna) == 0){
                            fim = 0;
                            posicao = i;
                            tipo = Colunas[i].tipo;
                            not_null = Colunas[i].not_null;
                        }
                    }
                    if(fim == 1) printf("Esse campo não existe\n");
                }
                free(Colunas);
            }
            fclose(arquivo);
            pesquisar_registro(nome, posicao, tipo, not_null);
        }
        while(inser != 0){
            printf("\nDeseja realizar outra busca na tabela %s?\n1-Sim 0-Não\n", nome);
            scanf("%s", campo);
            getchar();
            if (strcmp(campo, "/cancelar") == 0) { limpar(); return; }
            if(checar_inteiro(campo) == 1) loop = converter_string_em_inteiro(campo);

            if(checar_inteiro(campo) == 1 && loop >= 0 && loop <= 1){
                inser = 0;
            }
            else{
                printf("Insira um valor válido");
            }
        }
        inser = 1;
    }
    free(campo);
    free(nome);
}
//recebe o valor a ser pesquisado e o modo de pesquisa
void pesquisar_registro(char *nome, int posicao, int tipo, int not_null){
    char *valor, *recebimento;
    valor = malloc(TAMANHO);
    recebimento = malloc(TAMANHO);
    if(nome == NULL || recebimento == NULL){
        fprintf(stderr, "Erro na alocação de memória \n");
    }
    int x = 1, fim = 0, comp, conteudo = 0;
    printf("Insira o valor a ser pesquisado\n");
    receber_campo(valor, tipo, 1);
    if (strcmp(valor, "/cancelar") == 0) { limpar(); printf("----- PESQUISAR VALOR -----\n"); return; }
    limpar();
    printf("Conteudo da tabela:\n");
    conteudo = listar_conteudo(nome);
    if(conteudo == -1) {
        free(valor);
        free(recebimento);
        return;
    }
    else{
        printf("\n");
        while(x != 0){
            printf("----- PESQUISAR VALOR -----\n");
            printf("Valor inserido: %s\n\n", valor);
            printf("Escolha a opção para a pesquisa:\n");
            printf("1-Valores maiores que o valor informado\n");
            printf("2-Valores maiores que ou iguais ao valor informado\n");
            printf("3-Valores iguais ao valor informado\n");
            printf("4-Valores menores que o valor informado\n");
            printf("5-Valores menores que ou iguais ao valor informado\n");
            if(tipo == 5) printf("6-Valores próximos ao valor informado\n");
            printf("0-Parar pesquisa\n");
            receber_campo(recebimento, 2, 1);
            if (strcmp(recebimento, "/cancelar") == 0) { limpar(); printf("----- PESQUISAR VALOR -----\n"); return; }
            if(checar_inteiro(recebimento) == 1) x = converter_string_em_inteiro(recebimento);
            if(checar_inteiro(recebimento) != 1 || (tipo != 5 && (x < 0 || x > 5)) || (tipo == 5 && (x < 0 || x > 6))){
                limpar();
                printf("Opção inválida\n\n");
            }
            else if(x != 0){
                limpar();
                printf("----- RESULTADO -----\n");
                realizar_busca(nome, valor, posicao, tipo, x);
            }
        }
        free(valor);
        free(recebimento);
    }
}
//realiza uma busca na tabela pelo valor desejado no campo desejado e o modo da pesquisa
void realizar_busca(char *nome, char *valor, int posicao, int tipo, int x){
    char *comparador, *lixo, *teste, a, b, c, d;
    teste = malloc(TAMANHO);
    comparador = malloc(TAMANHO);
    lixo = malloc(TAMANHO);
    if(teste == NULL || comparador == NULL || lixo == NULL){
        fprintf(stderr, "Erro na alocação de memória \n");
    }
    int cont = 0, qtdColunas = contar_colunas(nome), qtdLinhas = contar_linhas(nome), alinhar;
    int espacos[qtdColunas];
    for (int i = 0; i < qtdColunas; i++){
        espacos[i] = espacos_por_coluna(nome, qtdColunas, i);
    }
    double y, z, numero;
    FILE *leitura, *mostrar;
    alocar_arquivo(&leitura, nome, "r");
    alocar_arquivo(&mostrar, nome, "r");
    if(leitura == NULL || mostrar == NULL){
        fprintf(stderr, "Erro na abertura do arquivo %s\n", nome);
        return;
    }
    if(tipo == 2 || tipo == 3 || tipo == 4) sscanf(valor, "%lf", &y);
    fscanf(leitura, "%s\n", nome);
    fscanf(leitura, "%c", &b);
    a = (char)b;
    while(a != '\n' && !feof(leitura)){
        fscanf(leitura, "%c", &b);
        a = (char) b;
    }
    fscanf(mostrar, "%s\n", nome);
    fscanf(mostrar, "%c", &d);
    c = (char)d;
    while(c != '\n' && !feof(mostrar)){
        fscanf(mostrar, "%c", &d);
        c = (char) d;
    }
    escrever_cabecalho(nome);
    for(int k = 0; k < qtdLinhas; k++){
        for(int i = 0; i < qtdColunas; i++){
            if(i == posicao){
                strcpy(comparador, escanear_campo(nome, k, i).nome);
                sscanf(comparador, "%lf", &z);
                if(x == 1){
                    for(int j = 0; j < qtdColunas; j++){
                        strcpy(teste, escanear_campo(nome, k, j).nome);
                        if((tipo == 2 || tipo == 3 || tipo == 4) && z > y) {
                            if(j == 0) printf("| ");
                            cont++; 
                            printf("%s ", teste);
                            alinhar = espacos[j] - strlen(teste);
                            for (int l = 0; l < alinhar; l++) {
                                printf(" ");
                            }
                            printf("| ");
                        }
                        else if(tipo == 1 && comparador[0] > valor[0]) {
                            if(j == 0) printf("| ");
                            cont++; 
                            printf("%s ", teste);
                            alinhar = espacos[j] - strlen(teste);
                            for (int l = 0; l < alinhar; l++) {
                                printf(" ");
                            }
                            printf("| ");
                        }
                        else if(tipo == 5 && strcmp(comparador, valor) > 0) {
                            if(j == 0) printf("| ");
                            cont++; 
                            printf("%s ", teste);
                            alinhar = espacos[j] - strlen(teste);
                            for (int l = 0; l < alinhar; l++) {
                                printf(" ");
                            }
                            printf("| ");
                        }
                    }
                    if((tipo == 2 || tipo == 3 || tipo == 4) && z > y) printf("\n");
                    else if(tipo == 1 && comparador[0] > valor[0]) printf("\n");
                    else if(tipo == 5 && strcmp(comparador, valor) > 0) printf("\n");
                }
                else if(x == 2){
                    for(int j = 0; j < qtdColunas; j++){
                        strcpy(teste, escanear_campo(nome, k, j).nome);
                        if((tipo == 2 || tipo == 3 || tipo == 4) && z >= y) {
                            if(j == 0) printf("| ");
                            cont++; 
                            printf("%s ", teste);
                            alinhar = espacos[j] - strlen(teste);
                            for (int l = 0; l < alinhar; l++) {
                                printf(" ");
                            }
                            printf("| ");
                        }
                        else if(tipo == 1 && comparador[0] >= valor[0]) {
                            if(j == 0) printf("| ");
                            cont++; 
                            printf("%s ", teste);
                            alinhar = espacos[j] - strlen(teste);
                            for (int l = 0; l < alinhar; l++) {
                                printf(" ");
                            }
                            printf("| ");
                        }
                        else if(tipo == 5 && strcmp(comparador, valor) >= 0) {
                            if(j == 0) printf("| ");
                            cont++; 
                            printf("%s ", teste);
                            alinhar = espacos[j] - strlen(teste);
                            for (int l = 0; l < alinhar; l++) {
                                printf(" ");
                            }
                            printf("| ");
                        }
                    }
                    if((tipo == 2 || tipo == 3 || tipo == 4) && z >= y) printf("\n");
                    else if(tipo == 1 && comparador[0] >= valor[0]) printf("\n");
                    else if(tipo == 5 && strcmp(comparador, valor) >= 0) printf("\n");
                }
                else if(x == 3){
                    for(int j = 0; j < qtdColunas; j++){
                        strcpy(teste, escanear_campo(nome, k, j).nome);
                        if((tipo == 2 || tipo == 3 || tipo == 4) && z == y) {
                            if(j == 0) printf("| ");
                            cont++; 
                            printf("%s ", teste);
                            alinhar = espacos[j] - strlen(teste);
                            for (int l = 0; l < alinhar; l++) {
                                printf(" ");
                            }
                            printf("| ");
                        }
                        else if(tipo == 1 && comparador[0] == valor[0]) {
                            if(j == 0) printf("| ");
                            cont++; 
                            printf("%s ", teste);
                            alinhar = espacos[j] - strlen(teste);
                            for (int l = 0; l < alinhar; l++) {
                                printf(" ");
                            }
                            printf("| ");
                        }
                        else if(tipo == 5 && strcmp(comparador, valor) == 0) {
                            if(j == 0) printf("| ");
                            cont++; 
                            printf("%s ", teste);
                            alinhar = espacos[j] - strlen(teste);
                            for (int l = 0; l < alinhar; l++) {
                                printf(" ");
                            }
                            printf("| ");
                        }
                    }
                    if((tipo == 2 || tipo == 3 || tipo == 4) && z == y) printf("\n");
                    else if(tipo == 1 && comparador[0] == valor[0]) printf("\n");
                    else if(tipo == 5 && strcmp(comparador, valor) == 0) printf("\n");
                }
                else if(x == 4){
                    for(int j = 0; j < qtdColunas; j++){
                        strcpy(teste, escanear_campo(nome, k, j).nome);
                        if((tipo == 2 || tipo == 3 || tipo == 4) && z < y) {
                            if(j == 0) printf("| ");
                            cont++; 
                            printf("%s ", teste);
                            alinhar = espacos[j] - strlen(teste);
                            for (int l = 0; l < alinhar; l++) {
                                printf(" ");
                            }
                            printf("| ");
                        }
                        else if(tipo == 1 && comparador[0] < valor[0]) {
                            if(j == 0) printf("| ");
                            cont++; 
                            printf("%s ", teste);
                            alinhar = espacos[j] - strlen(teste);
                            for (int l = 0; l < alinhar; l++) {
                                printf(" ");
                            }
                            printf("| ");
                        }
                        else if(tipo == 5 && strcmp(comparador, valor) < 0) {
                            if(j == 0) printf("| ");
                            cont++; 
                            printf("%s ", teste);
                            alinhar = espacos[j] - strlen(teste);
                            for (int l = 0; l < alinhar; l++) {
                                printf(" ");
                            }
                            printf("| ");
                        }
                    }
                    if((tipo == 2 || tipo == 3 || tipo == 4) && z < y) printf("\n");
                    else if(tipo == 1 && comparador[0] < valor[0]) printf("\n");
                    else if(tipo == 5 && strcmp(comparador, valor) < 0) printf("\n");
                }
                else if(x == 5){
                    for(int j = 0; j < qtdColunas; j++){
                        strcpy(teste, escanear_campo(nome, k, j).nome);
                        if((tipo == 2 || tipo == 3 || tipo == 4) && z <= y) {
                            if(j == 0) printf("| ");
                            cont++; 
                            printf("%s ", teste);
                            alinhar = espacos[j] - strlen(teste);
                            for (int l = 0; l < alinhar; l++) {
                                printf(" ");
                            }
                            printf("| ");
                        }
                        else if(tipo == 1 && comparador[0] <= valor[0]) {
                            if(j == 0) printf("| ");
                            cont++; 
                            printf("%s ", teste);
                            alinhar = espacos[j] - strlen(teste);
                            for (int l = 0; l < alinhar; l++) {
                                printf(" ");
                            }
                            printf("| ");
                        }
                        else if(tipo == 5 && strcmp(comparador, valor) <= 0) {
                            if(j == 0) printf("| ");
                            cont++; 
                            printf("%s ", teste);
                            alinhar = espacos[j] - strlen(teste);
                            for (int l = 0; l < alinhar; l++) {
                                printf(" ");
                            }
                            printf("| ");
                        }
                    }
                    if((tipo == 2 || tipo == 3 || tipo == 4) && z <= y) printf("\n");
                    else if(tipo == 1 && comparador[0] <= valor[0]) printf("\n");
                    else if(tipo == 5 && strcmp(comparador, valor) <= 0) printf("\n");
                }
                else if(x == 6 && tipo == 5){
                    if(compara_string_proxima(valor, comparador) < 2 && strcmp(comparador, "") != 0){
                        for(int j = 0; j < qtdColunas; j++){
                            strcpy(teste, escanear_campo(nome, k, j).nome);
                            if(j == 0) printf("| ");
                            cont++; 
                            printf("%s ", teste);
                            alinhar = espacos[j] - strlen(teste);
                            for (int l = 0; l < alinhar; l++) {
                                printf(" ");
                            }
                            printf("| ");
                        }
                        printf("\n");
                    }
                }
            }
        }
    }
    if(cont == 0) {
        limpar();
        printf("----- RESULTADO -----\n");
        printf("Nenhum valor encontrado\n");
    }
    else{
        listar_estilo_linha(qtdColunas, espacos);
    }
    printf("\n");
    fclose(leitura);
    fclose(mostrar);
    free(teste);
    free(lixo);
    free(comparador);
}
//recebe um campo e verifica sua autenticidade
void receber_campo(char *valor, int tipo, int not_null){
    int validarCampo = 0;
    while (validarCampo != 1){
        if(tipo == 5){
            fgets(valor, TAMANHO, stdin);
            if(strlen(valor) > 0 && (valor[strlen(valor)-1] == '\n')){
                valor[strlen(valor)-1] = '\0';
            }
        }
        else{
            scanf("%s", valor);
            getchar();
        }
        if (strcmp(valor, "/cancelar") == 0) return;
        if (strcmp(valor, "null") == 0 && not_null == 1){
            printf("Essa coluna não aceita valores nulos\n");
            validarCampo = 0;
        }
        else if (strcmp(valor, "null") != 0){
            validarCampo = checar_chamada_campo(valor, tipo);
            if (validarCampo == 0) {
                if(tipo == 1) printf("Insira um conteúdo do tipo char\n");
                if(tipo == 2) printf("Insira um conteúdo do tipo int\n");
                if(tipo == 3) printf("Insira um conteúdo do tipo float\n");
                if(tipo == 4) printf("Insira um conteúdo do tipo double\n");
                if(tipo == 5) printf("Insira um conteúdo do tipo string\n");
            }
        }
        else validarCampo = 1;
    }
}
//recebe uma chave primária e verifica sua autenticidade
void receber_chave_primaria(char *valor){
    int validarCampo = 0;
    while (validarCampo != 1){
        scanf("%s", valor);
        getchar();
        if (strcmp(valor, "/cancelar") == 0) return;
        if (strcmp(valor, "null") == 0){
            printf("Essa coluna não aceita valores nulos\n");
            validarCampo = 0;
        }
        else{
            validarCampo = checar_chamada_campo(valor, 2);
            if (validarCampo == 0) printf("Insira um conteúdo do tipo inteiro\n");
            else if(converter_string_em_inteiro(valor) < 0){
                printf("Insira um valor maior ou igual a zero\n");
                validarCampo = 0;
            }
        }
    }
}
//recebe o nome de uma tabela e verifica sua existência conforme a necessidade
void receber_nome_tabela(char *nome, int i){
    int fim;
    //i = 0 para caso necessite que não haja a tabela criada, i = 1 para caso necessite que haja a tabela criada
    if(i == 0){
        fim = 0;
        while(fim!=1){
            printf("\nInsira o nome da tabela\n");
            scanf("%s", nome);
            getchar();
            if (strcmp(nome, "/cancelar") == 0) {
                return;
            }
            fim = checar_nome_tabela(nome);
            if(fim == 0) printf("Essa tabela ja existe\n");
        }
    }
    else{
        fim = 1;
        while(fim!=0){
            printf("\nInsira o nome da tabela\n");
            scanf("%s", nome);
            getchar();
            if (strcmp(nome, "/cancelar") == 0) {
                return;
            }
            fim = checar_nome_tabela(nome);
            if(fim == 1) printf("Essa tabela não existe.\n");
        }
    }
}
//retorna o tipo da coluna da tabela
int tipo_coluna(char *nome, int posicao){
    char *valor = malloc(TAMANHO);
    if(valor == NULL){
        fprintf(stderr, "Erro na alocação de memória \n");
    }
    int qtdColunas = contar_colunas(nome);
    FILE *lerColunas = NULL; 
    alocar_arquivo(&lerColunas, nome, "r");
    if(lerColunas == NULL){
        fprintf(stderr, "Erro na abertura do arquivo %s\n", nome);
    }
    coluna Coluna;
    fscanf(lerColunas, "%s\n", valor);
    for (int i = 0; i < qtdColunas; i++){
        fscanf(lerColunas, "%d %d %d %s | ", &Coluna.tipo, &Coluna.ai, &Coluna.not_null, Coluna.nome_coluna);
        if (i == posicao) {
            free(valor);
            fclose(lerColunas);
            return Coluna.tipo;
        } 
    }
}
//transcreve colunas do arquivo de leitura para o de escrita provisória
void transcrever_colunas(FILE *leitura, FILE *escritaProvisoria, coluna *Coluna){
    fscanf(leitura, "%d %d %d %s | ", &(Coluna->tipo), &(Coluna->ai), &(Coluna->not_null), Coluna->nome_coluna);
    fprintf(escritaProvisoria, "%d %d %d %s | ", Coluna->tipo, Coluna->ai, Coluna->not_null, Coluna->nome_coluna);
}
//retorna o último id(ou chave primária de outro nome, com auto-incremento) da tabela
int ultimo_id_tabela(char *nome){
    char c, a;
    int cont = 0, chave = 0;
    FILE *arquivo;
    alocar_arquivo(&arquivo, nome, "a+");
    if(arquivo == NULL){
        fprintf(stderr, "Erro na abertura do arquivo %s\n", nome);
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
//verifica se já existe uma chave primária com o mesmo valor que o inserido
int verificar_chave(char *nome, char *valor){
    char a, c, *chave;
    chave = malloc(TAMANHO);
    if(chave == NULL){
        fprintf(stderr, "Erro na alocação de memória \n");
    }
    FILE *arquivo;
    alocar_arquivo(&arquivo, nome, "r");
    if(arquivo == NULL){
        fprintf(stderr, "Erro na abertura do arquivo %s\n", nome);
    }
    fscanf(arquivo, "%s\n", nome);
    fscanf(arquivo, "%c", &c);
    a = (char)c;
    while(a != '\n' && !feof(arquivo)){
        fscanf(arquivo, "%c", &c);
        a = (char) c;
    }
    if(feof(arquivo)){
        fclose(arquivo);
        return 0;
    } 
    while(!feof(arquivo)){
        fscanf(arquivo, "%s | ", chave);
        if(strcmp(chave, valor) == 0) {
            fclose(arquivo);
            return 1;
        }
        else{
            fscanf(arquivo, "%c", &c);
            a = (char) c;
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