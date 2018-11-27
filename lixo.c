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
        //retorno 1 caso não haja nenhum arquivo com o nome igual ao parâmetro
        return 1;
    }  
}
int contar_linhas(char nome[100], int qtd){
    coluna Coluna;
    int cont = 0;
    char c, a;
    char provisorio[100];
    strcpy(provisorio, "./tabelas/");
    strcat(provisorio, nome);
    //criação de arquivo com o nome do parâmetro + ".txt"
    FILE *leitura = fopen(strcat(provisorio, ".txt"), "r");
    for(int i = 0; i < qtd; i++){
        fscanf(leitura, "%d %d %d %s | ", &Coluna.tipo, &Coluna.ai, &Coluna.not_null, Coluna.nome_coluna);
    }
    fscanf(leitura, "\n\n");
    while(!feof(leitura)){
        fscanf(leitura, "%c", &c);
        a = (char) c;
        if(a == '\n') cont++;
    }
    printf("c\n");
    return cont-2;
}
void abrir_arquivo(){
    char provisorio[100];
    strcpy(provisorio, "./tabelas/");
    strcat(provisorio, nome);
    //criação de arquivo com o nome do parâmetro + ".txt"
    FILE *arquivo = fopen(strcat(provisorio, ".txt"), "w");
}
int contar_linhas(char nome[100]){
    int cont = 0;
    char c, a;
    alocar_arquivo(&arquivo, nome, 'r');
    while(!feof(arquivo)){
        fscanf(arquivo, "%c", &c);
        a = (char) c;
        if(a == '\n') cont++;
    }
    return cont-2;
}
void mostrar_num_caracteres(){
    /*for(int i = 0; i < cont; i++){
        printf("%d ", caracteres[i]);   
    }*/
    //printf("\n");
}
void apagar_registro(){
    char *nome, *valor, *registro, *campo, c, a;
    nome = malloc(TAMANHO);
    valor = malloc(TAMANHO);
    campo = malloc(TAMANHO);
    int fim = 0, validarCampo = 0, qtdColunas = 0, qtdLinhas = 0, escreverLinha = 1, quebrarLinha = 0, tipo = 0, cont = 0;
    FILE *escritaProvisoria = fopen("provisorios/tabela_provisoria.txt", "w+");
    FILE *leituraProvisoria = fopen("provisorios/tabela_provisoria.txt", "r");
    FILE *leitura;
    coluna Coluna;
    receber_nome_tabela(nome, 1);
    alocar_arquivo(&leitura, nome, "r");
    while (fscanf(leitura, "%c", &c) != EOF) {
        a = (char) c;
        if(a == '\n') {
            qtdLinhas++;
        }
    }
    fseek(leitura, 0, SEEK_SET);
    if(escritaProvisoria == NULL || leituraProvisoria == NULL || leitura == NULL) printf("Erro na abertura de arquivo\n");
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
void apagar_registro(){
    char *nome, *valor, *registro, *campo, c, a;
    nome = malloc(TAMANHO);
    valor = malloc(TAMANHO);
    campo = malloc(TAMANHO);
    int fim = 0, validarCampo = 0, qtdColunas = 0, qtdLinhas = 0, escreverLinha = 1, quebrarLinha = 0, cont = 0;
    FILE *escritaProvisoria = fopen("provisorios/tabela_provisoria.txt", "w+");
    FILE *leituraProvisoria = fopen("provisorios/tabela_provisoria.txt", "r");
    FILE *leitura;
    coluna Coluna;
    receber_nome_tabela(nome, 1);
    alocar_arquivo(&leitura, nome, "r");
    while (fscanf(leitura, "%c", &c) != EOF) {
        a = (char) c;
        if(a == '\n') {
            qtdLinhas++;
        }
    }
    fseek(leitura, 0, SEEK_SET);
    if(escritaProvisoria == NULL || leituraProvisoria == NULL || leitura == NULL) printf("Erro na abertura de arquivo\n");
    else{
        fscanf(leitura, "%s\n", nome);
        fprintf(escritaProvisoria, "%s\n", nome);
        qtdColunas = ler_tabela(nome);
        for (int i = 0; i < qtdColunas; i++){
            fscanf(leitura, "%d %d %d %s | ", &Coluna.tipo, &Coluna.ai, &Coluna.not_null, Coluna.nome_coluna);
            fprintf(escritaProvisoria, "%d %d %d %s | ", Coluna.tipo, Coluna.ai, Coluna.not_null, Coluna.nome_coluna);
            //transcrever_colunas(leitura, escritaProvisoria, Coluna);
        }

        while(fim!=1){
            printf("Insira a chave primaria do registro que deseja apagar:\n");
            while (validarCampo != 1){
                scanf("%s", valor);
                validarCampo = checar_chamada_campo(valor, 2);
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