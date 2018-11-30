#include "math.h"
#include "checagem.h"
#include <stdio.h>
#define TAMANHO 100

//todas as checagens foram feitas por comparação com a tabela ASCII 

//checa se a string inserida é um char
int checar_char(char *nome){
    if (strcmp(nome, "null") != 0){
        int tamanho = strlen(nome);
        if (tamanho > 1) return 0;
    }
    return 1;
}
//checa se a string inserida é um float (ou double)
int checar_float(char *nome){
    if (strcmp(nome, "null") != 0){
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
    }
    return 1;
}
//checa se a string inserida é um int
int checar_inteiro(char *nome){
    if (strcmp(nome, "null") != 0){
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
    }
    return 1;
}
//recebe um valor e seu suposto tipo e checa se é do tipo correto
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
//checa se já existe uma tabela com o mesmo nome que o valor inserido
int checar_nome_tabela(char *nome){
    char *tabela;
    tabela = malloc(TAMANHO);
    //abertura de arquivo tabelas.txt para leitura 
    FILE *arquivo;
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