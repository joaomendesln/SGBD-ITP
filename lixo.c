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