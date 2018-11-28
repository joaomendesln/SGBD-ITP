#include "functions.h"
#include "checagem.h"
#include "convert.h"
#include <stdio.h>
#define TAMANHO 100

//aloca o ponteiro de arquivo desejado para o arquivo de nome inserido no modo inserido
void alocar_arquivo(FILE** ptr, char *nome, char modo[]){
    char *provisorio = malloc(TAMANHO);
    strcpy(provisorio, "./tabelas/");
    strcat(provisorio, nome);
    //criação de arquivo com o nome do parâmetro + ".txt"
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
    int fim = 0, validarCampo = 0, qtdColunas = 0, qtdLinhas = 0, escreverLinha = 1, quebrarLinha = 0, cont = 0, conteudo = 0;
    //alocando ponteiro para uma tabela previamente definida para escrever o conteudo da tabela solicitada, antes de excluir um dos registros
    FILE *escritaProvisoria = fopen("provisorios/tabela_provisoria.txt", "w+");
    FILE *leitura;
    //recebendo nome da tabela e alocando um ponteiro de arquivo com seu nome para leitura
    receber_nome_tabela(nome, 1);
    if (strcmp(nome, "/cancelar") == 0) { limpar(); return; }
    alocar_arquivo(&leitura, nome, "r");
    limpar();
    //listagem de todos os elementos da tabela para facilitar a procura do usuário e verificando se existem registros na tabela escolhida
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
            //contando a quantidade de registros
            while (fscanf(leitura, "%c", &c) != EOF) {
                a = (char) c;
                if(a == '\n') {
                    qtdLinhas++;
                }
            }
            qtdLinhas--;
            fseek(leitura, 0, SEEK_SET);
            //lendo a primeira linha do ponteiro de leitura (nome da tabela) e escrevendo-a no arquivo provisório
            fscanf(leitura, "%s\n", nome);
            fprintf(escritaProvisoria, "%s\n", nome);
            //obtendo a quantidade de colunas e transcrevendo as colunas para o arquivo provisório
            qtdColunas = contar_colunas(nome);
            for (int i = 0; i < qtdColunas; i++){
                transcrever_colunas(leitura, escritaProvisoria, &Coluna);
            }
            //recebendo a chave primária do registro a ser apagado e verificando sua autenticidade
            while(fim!=1){
                printf("Insira a chave primaria do registro que deseja apagar:\n");
                receber_chave_primaria(valor);
                if (strcmp(valor, "/cancelar") == 0) { limpar(); return; }
                fim = verificar_chave(nome, valor);
                if(fim == 0) printf("Esse valor não existe\n");
            }
            //se a tabela tiver apenas um registro(por consequência, após este ser apagado não haverá mais nenhum registro), não será adicionado...
            //...um "\n" ao arquivo provisório, já que só haverá quebra de linha após a linha de colunas se houver pelo menos um registro
            if(qtdLinhas >= 2) fprintf(escritaProvisoria, "\n");
            //enquanto o arquivo do ponteiro não chegar ao fim, o laço continuará rodando
            while (!feof(leitura)){
                //laço para escrever o valor de cada coluna de cada registro
                for (int i = 0; i < qtdColunas; i++){
                    //leitura do valor do campo no arquivo de leitura
                    fscanf(leitura, "%s | ", campo);
                    //se a chave primária do registro da linha atual for igual à inserida, a linha não será escrita no arquivo provisório
                    if(i == 0 && strcmp(valor, campo) == 0)    
                        escreverLinha = 0;
                    else{
                        //se a chave primária do registro da linha atual for diferente da inserida, a linha será escrita no arquivo provisório
                        if (escreverLinha == 1){
                            fprintf(escritaProvisoria, "%s | ", campo);
                            //se a linha for escrita, será adicionado uma quebra de linha ao final
                            quebrarLinha = 1;
                        }
                    }
                }
                cont++;
                //não haverá quebra de linha nas duas ultimas iterações, pois: (1) foi inserido uma linha a menos que o arquivo portanto será uma...
                //...quebra de linha a menos (2) não há quebra de linha na última linha do arquivo na nossa formatação, então menos outra inserção
                if (quebrarLinha == 1 && cont < qtdLinhas-1) fprintf(escritaProvisoria, "\n");
                escreverLinha = 1;
                quebrarLinha = 0;
                fscanf(leitura, "\n");
            }
            fclose(leitura);
            fclose(escritaProvisoria);
            //alocando ponteiro para ler os dados inseridos no arquivo provisório e outro ponteiro para escrever no arquivo original
            FILE *leituraProvisoria = fopen("provisorios/tabela_provisoria.txt", "r");
            FILE *sobrescreverTabela;
            alocar_arquivo(&sobrescreverTabela, nome, "w+");
            //caso de erro: arquivo não abre
            if(leituraProvisoria == NULL || sobrescreverTabela == NULL){
                printf("Erro na abertura de arquivo\n");
                return;
            }
            //lendo todos os caracteres do arquivo provisorio e escrevendo no arquivo original
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
    int status;
    //alocando ponteiro para um arquivo previamente definido para escrever o conteudo do arquivo com o nome das tabelas, antes de excluir uma das tabelas
    FILE *escritaProvisoria = fopen("provisorios/lista_provisoria.txt", "w+");
    //alocando ponteiro para o arquivo com os nomes das tabelas
    FILE *listaTabelas = fopen("tabelas.txt", "r");
    //recebendo nome da tabela a ser excluida
    receber_nome_tabela(nome, 1);
    if (strcmp(nome, "/cancelar") == 0) { limpar(); return; }
    //caso de erro: arquivo não abre
    if(listaTabelas == NULL || escritaProvisoria == NULL){
        printf("Erro na abertura do arquivo\n");
        return;
    }
    //leitura dos nomes das tabelas até o fim do arquivo
    else{
        //lendo todos os nomes das tabelas existentes e os copiando para o arquivo provisório, caso seja igual ao nome inserido, o programa não copiará o nome para o arquivo provisório
        while(fscanf(listaTabelas, "%s\n", nomeTabela) != EOF){
            if(strcmp(nome, nomeTabela) != 0) {
                fprintf(escritaProvisoria, "%s\n", nomeTabela);
            }
        }
        fclose(escritaProvisoria);
        fclose(listaTabelas);
        //alocando um ponteiro para ler informações do arquivo provisorio e outro para escrever no arquivo original
        FILE *leituraProvisoria = fopen("provisorios/lista_provisoria.txt", "r");
        FILE *sobrescreverTabelas = fopen("tabelas.txt", "w+");
        //caso de erro: arquivo não abre
        if(leituraProvisoria == NULL || sobrescreverTabelas == NULL){
            printf("Erro na abertura do arquivo\n");
            return;
        }
        //concatenando o caminho do arquivo, o nome do arquivo e a extensão .txt
        strcpy(nomeArquivo, "./tabelas/");
        strcat(nomeArquivo, nome);
        strcat(nomeArquivo, ".txt");
        //excluindo a tabela
        status = remove(nomeArquivo);
        limpar();
        if (status == 0) printf("Tabela apagada com sucesso!\n");

        while(fscanf(leituraProvisoria, "%s\n", nomeTabela) != EOF){
            fprintf(sobrescreverTabelas, "%s\n", nomeTabela);
        }
        //lendo todos os caracteres do arquivo provisorio e escrevendo no arquivo original
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
    int qtdColunas = 0, conteudo = 0, fim = 0, posicao, tipo, linha, cont = 1, not_null;
    //alocando ponteiro para uma tabela previamente definida para escrever o conteudo da tabela solicitada, antes de alterar um dos registros
    FILE *escritaProvisoria = fopen("provisorios/tabela_provisoria.txt", "w+");
    FILE *leitura;
    //recebendo nome da tabela e alocando um ponteiro de arquivo com seu nome para leitura
    receber_nome_tabela(nome, 1);
    if (strcmp(nome, "/cancelar") == 0) { limpar(); return; }
    alocar_arquivo(&leitura, nome, "r");
    //caso de erro: arquivo não abre
    if(leitura == NULL || escritaProvisoria == NULL){
        printf("Erro na abertura de arquivo\n");
        return;
    }
    printf("\n");
    limpar();
    //listagem de todos os elementos da tabela para facilitar a procura do usuário e verificando se existem registros na tabela escolhida
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
        //caso haja apenas uma coluna, não há colunas que o usuário poderá alterar, já que a primeira coluna será a chave primária e a mesma não pode ser alterada
        if(qtdColunas == 1) printf("Não há colunas disponíveis para alteração\n");
        else{
            //alocando um vetor de structs do tipo coluna, para salvar as especificações das colunas ao mesmo tempo que as transfere para o arquivo provisório
            Colunas = (coluna *) malloc(qtdColunas*sizeof(coluna));
            fseek(leitura, 0, SEEK_SET);
            fscanf(leitura, "%s\n", nome);
            fprintf(escritaProvisoria, "%s\n", nome);
            for(int i = 0; i < qtdColunas; i++){
                transcrever_colunas(leitura, escritaProvisoria, &Colunas[i]);
            }
            //recebendo a chave primária do registro a ser alterado e verificando sua autenticidade
            while(fim!=1){
                printf("Insira a chave primaria \"%s\" da coluna que deseja alterar\n", Colunas[0].nome_coluna);
                receber_chave_primaria(valor);
                if (strcmp(valor, "/cancelar") == 0) { limpar(); return; }
                fim = verificar_chave(nome, valor);
                if(fim == 0) printf("Não há uma chave primária com esse valor\n");
            }
            fim = 0;
            //mostrando o registro completo para o usuário para que cheque novamente os campos e tenha certeza do registro a ser alterado, ao mesmo tempo, a linha do registro é obtida
            printf("Registro encontrado:\n");
            linha = listar_registro(nome, valor);
            //caso a função tenha retornado -1, significa que não há registros na tabela, portanto não há como atualizar um registro
            if(linha == -1) return;
            //obtendo o nome da coluna que será alterada e verificando se o campo inserido existe e pode ser alterado
            while(fim!=1){
                printf("\nInsira a coluna que deseja alterar\n");
                scanf("%s", campo);
                for(int i = 1; i < qtdColunas; i++){
                    //ao encontrar um campo que seja igual ao inserido, a posição desse campo(ordem de escrita), seu tipo e sua capacidade de aceitar valores nulos são salvos
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
            //recebendo novo valor para a coluna a ser alterada
            printf("Insira o novo conteudo para a coluna \"%s\"\n", campo);
            receber_campo(valor, tipo, not_null);
            //laço para rodar até o fim do arquivo leitura
            while (!feof(leitura)){
                //inserção da quebra de linha no arquivo provisório antes de cada registro
                fprintf(escritaProvisoria, "\n");
                //o laço vai rodar tantas vezes quanto o número de colunas da tabela, para que possamos inserir cada valor de coluna separadamente
                for(int i = 0; i < qtdColunas; i++){
                    //lendo valor por valor no arquivo original
                    fscanf(leitura, "%s | ", campo);
                    //ao chegar na mesma linha e na mesma posição do campo que foi alterado, o programa escreverá o novo valor no arquivo provisório
                    if(cont == linha && i == posicao){
                        fprintf(escritaProvisoria, "%s | ", valor);
                    }
                    //em todos os outros casos, o programa escreverá o valor antigo(já que todos os outros campos permanecem inalterados)
                    else{
                        fprintf(escritaProvisoria, "%s | ", campo);
                    }
                }
                cont++;
            }
            fclose(escritaProvisoria);
            fclose(leitura);
            //alocando ponteiro para ler os dados inseridos no arquivo provisório e outro ponteiro para escrever no arquivo original
            FILE *leituraProvisoria = fopen("provisorios/tabela_provisoria.txt", "r");
            FILE *sobrescreverTabela;
            alocar_arquivo(&sobrescreverTabela, nome, "w+");
            //caso de erro: arquivo não abre
            if(leituraProvisoria == NULL || sobrescreverTabela == NULL){
                printf("Erro na abertura de arquivo\n");
                return;
            }
            //lendo todos os caracteres do arquivo provisorio e escrevendo no arquivo original
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
    int cont = 0, fim = 1, validarCampo = 0, loop = 1, inser = 1;
    //criação de ponteiros para escrita e leitura com o nome do parâmetro + ".txt"
    FILE *leitura, *escrita;
    coluna Coluna;
    while(loop != 0){
        alocar_arquivo(&leitura, nome, "r");
        alocar_arquivo(&escrita, nome, "a");
        fseek(leitura, 0, SEEK_SET);
        fseek(escrita, 0, SEEK_SET);
        //pulando a primeira linha (há apenas o nome da tabela)
        fscanf(leitura, "%s\n", nome);
        //inserindo uma quebra de linha para inserir as próximas informações
        fprintf(escrita, "\n");
        for(int i = 0; i < qtdColunas; i++){
            fscanf(leitura, "%d %d %d %s | ", &Coluna.tipo, &Coluna.ai, &Coluna.not_null, Coluna.nome_coluna);
            if(strcmp(Coluna.nome_coluna, "id") != 0 && i != 0){
                printf("Insira o conteudo para a coluna \"%s\"\n", Coluna.nome_coluna);
                receber_campo(valor, Coluna.tipo, Coluna.not_null);
                if (strcmp(valor, "/cancelar") == 0) { limpar(); return; }
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
//usada no item 5, checa se uma string é "proxima" de outra string na tabela
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
//cria o arquivo
void criar_arquivo(char *nome){
    FILE *arquivo;
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
//insere as colunas e suas especificações no arquivo
void criar_coluna(coluna Coluna){
    FILE *arquivo;
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
//cria a tabela
void criar_tabela(){
	coluna *Colunas;
    char *nome = malloc(TAMANHO), *lixo = malloc(TAMANHO);
    int qtd, x, sid = 0, fim = 0;
    //estrutura para garantir que a tabela a ser criada ainda não exista
    receber_nome_tabela(nome, 0);
    if (strcmp(nome, "/cancelar") == 0) { limpar(); return; }
    while(fim!=1){
        printf("Insira o numero de colunas\n");
        receber_campo(lixo, 2, 1);
        if (strcmp(lixo, "/cancelar") == 0) { limpar(); return; }
        qtd = converter_string_em_inteiro(lixo);
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
        receber_campo(lixo, 2, 1);
        if (strcmp(lixo, "/cancelar") == 0) { limpar(); return; }
        sid = converter_string_em_inteiro(lixo);
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
                if (strcmp(Colunas[0].nome_coluna, "/cancelar") == 0) { limpar(); return; }
                while(fim!=1){
                    printf("Terá auto-incremento?\n1-Sim  0-Não\n");
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
    //criação dos Colunas da tabela
    printf("Insira o tipo e o nome de cada coluna\n1-char  2-int  3-float  4-double  5-string\nEx:'1 nome_Coluna'\n");
    for(int i = 2; i <= qtd; i++){
        while(fim != 1){
            printf("%dº - ", i);
            scanf("%s %s", lixo, Colunas[i-1].nome_coluna);
            if (strcmp(lixo, "/cancelar") == 0 || strcmp(Colunas[i-1].nome_coluna, "/cancelar") == 0) { limpar(); return; }
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
            printf("Aceitará valores nulos?\n1-Sim  0-Não\n");
            scanf("%s", lixo);
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
    limpar();
    printf("Tabela criada com sucesso!\n");
}
//escolhe o arquivo a ter seus dados listados e os lista
void escolher_listagem(){
    char *nome;
    nome =  malloc(TAMANHO);
    receber_nome_tabela(nome, 1);
    limpar();
    printf("Conteúdo da tabela:\n");
    listar_conteudo(nome);
    free(nome);
}
//conta o tamanho de caracteres do maior valor em uma coluna(seja o nome da coluna ou um registro)
int espacos_por_coluna(char *nome, int qtdColunas, int posicao){
    char *valor;
    valor = malloc(TAMANHO);
    int result = 0;
    FILE *arquivo;
    coluna Coluna;
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
                    if (strcmp(valor, "null") != 0 && strlen (valor) > result) result = strlen(valor);
                }
            }
            fscanf(arquivo, "\n");
        }
        return result;
    }
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
    int colunas;
    receber_nome_tabela(nome, 1);
    if (strcmp(nome, "/cancelar") == 0) { limpar(); return; }
    colunas = contar_colunas(nome);
    chamar_campos(nome, colunas);
}
//insere o nome de uma nova tabela na lista de tabelas
void inserir_nome(char *nome){
    //abertura de arquivo tabelas.txt, com listagem do nome de todas as tabelas, para escrita
    FILE *arquivo;
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
//conta quantas colunas uma tabela tem
int contar_colunas(char *nome){
    char teste, a;
    int cont = 0;
    FILE *arquivo;
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
//lista todo os registros de uma taela e retorna o numero de linhas
int listar_conteudo(char *nome){
    char a, c, *valor;
    valor = malloc(TAMANHO);
    FILE *listagem;
    coluna Coluna;
    alocar_arquivo(&listagem, nome, "r");
    int qtdColunas = contar_colunas(nome);
    int espacos[qtdColunas], alinhar = 0, hifens = 0;
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

        while (!feof(listagem)){
            for (int i = 0; i < qtdColunas; i++){
                if (i == 0) printf("| ");
                fscanf(listagem, "%s | ", valor);
                if (strcmp(valor, "null") == 0) {
                    printf(" ");
                    alinhar = espacos[i];
                }
                else{ 
                    printf("%s ", valor);
                    alinhar = espacos[i] - strlen(valor);
                }
                for (int j = 0; j < alinhar; j++) {
                    printf(" ");
                }
                printf("| ");

            }
            fscanf(listagem, "\n");
            printf("\n");
        }
        listar_estilo_linha(qtdColunas, espacos);
        fclose(listagem);
        printf("\n\n");
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
    int qtdColunas = contar_colunas(nome);
    int espacos[qtdColunas], alinhar = 0, hifens = 0, idRegistro = 0, validar = 0, cont = 0, linha;
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

        while (!feof(listagem)){
            for (int i = 0; i < qtdColunas; i++){
                fscanf(listagem, "%s | ", valor);
                if (i == 0 && strcmp(valor,id) == 0){
                    validar = 1; 
                }
                else {
                    if (i == 0 && strcmp(valor,id) != 0) {
                        validar = 0;
                        break;
                    }
                }
                if(validar == 1){
                    if (strlen(valor) > espacos[i]) espacos[i] = strlen(valor);
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
        fscanf(listagem, "\n");
        validar = 0;
        while (!feof(listagem)){
            cont++;
            for (int i = 0; i < qtdColunas; i++){
                fscanf(listagem, "%s | ", valor);
                if (i == 0 && strcmp(valor,id) == 0){
                    validar = 1;
                    linha = cont;
                }
                else {
                    if (i == 0 && strcmp(valor,id) != 0) validar = 0;
                }
                if(validar == 1){
                    if (i == 0) printf("| ");
                    if (strcmp(valor, "null") == 0) {
                        printf(" ");
                        alinhar = espacos[i];
                    }
                    else{ 
                        printf("%s ", valor);
                        alinhar = espacos[i] - strlen(valor);
                    }
                    for (int j = 0; j < alinhar; j++) {
                        printf(" ");
                    }
                    printf("| ");
                }
            }
            if (validar == 1) printf("\n");
            fscanf(listagem, "\n");
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
    int fim = 1, cont = 0, loop = 1, inser = 1, posicao, conteudo = 0, tipo;
    FILE *arquivo;
    receber_nome_tabela(nome, 1);
    printf("\n");
    while(loop != 0){
        alocar_arquivo(&arquivo, nome, "r");
        //caso de erro: arquivo não abre
        if(arquivo == NULL){
            printf("Erro na abertura do arquivo %s\n", nome);
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
            scanf("%s", campo);
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
void pesquisar_registro(char *nome, int posicao, int tipo){
    char *valor;
    valor = malloc(TAMANHO);
    int x = 1, fim = 0, comp, conteudo = 0;
    printf("Insira o valor a ser pesquisado\n");
    receber_campo(valor, tipo, 1);
    limpar();
    printf("Conteudo da tabela:\n");
    conteudo = listar_conteudo(nome);
    if(conteudo == -1) {
        free(valor);
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
            scanf("%d", &x);
            if((tipo != 5 && (x < 0 || x > 5)) || (tipo == 5 && (x < 0 || x > 6))){
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
    }
}
//realiza uma busca na tabela pelo valor desejado no campo desejado e o modo da pesquisa
void realizar_busca(char *nome, char *valor, int posicao, int tipo, int x){
    char *comparador, a, b, c, d;
    comparador = malloc(TAMANHO);
    int cont = 0;
    double y, numero;
    FILE *leitura, *mostrar;
    alocar_arquivo(&leitura, nome, "r");
    alocar_arquivo(&mostrar, nome, "r");
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
//recebe um campo e verifica sua autenticidade
void receber_campo(char *valor, int tipo, int not_null){
    int validarCampo = 0;
    while (validarCampo != 1){
        scanf("%s", valor);
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
            printf("Insira o nome da tabela\n");
            scanf("%s", nome);
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
            printf("Insira o nome da tabela\n");
            scanf("%s", nome);
            if (strcmp(nome, "/cancelar") == 0) {
                return;
            }
            fim = checar_nome_tabela(nome);
            if(fim == 1) printf("Essa tabela não existe.\n");
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
//verifica se já existe uma chave primária com o mesmo valor que o inserido
int verificar_chave(char *nome, char *valor){
    char a, c, *chave;
    chave = malloc(TAMANHO);
    FILE *arquivo;
    alocar_arquivo(&arquivo, nome, "r");
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