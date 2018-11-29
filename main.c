#include "functions.h"
#include "checagem.h"
#include "convert.h"
#include <stdio.h>
int main(void){
    limpar();
    char *valor = malloc(100);
    int x, fim=0, inser = 1;
    printf("Bem-vindo/a ao banco de dados\n");
    while(fim != 1){
        printf("\n----- MENU PRINCIPAL -----\n1-Criar tabela\n2-Listar todas as tabelas\n3-Inserir registro em uma tabela\n4-Listar dados de uma tabela\n5-Pesquisar valor em uma tabela\n6-Apagar um registro de uma tabela\n7-Apagar uma tabela\n8-Atualizar registro\n9-Ajuda\n0-Sair\n");
        receber_campo(valor, 2, 1);
        //getchar();
        if(checar_inteiro(valor) == 1){
            x = converter_string_em_inteiro(valor);
            switch(x){
                case 1:
                    limpar();
                    printf("----- CRIAR TABELA -----\n");
                    criar_tabela();
                    break;
                case 2:
                    limpar();
                    printf("----- LISTAR TABELAS -----\n");
                    listar();
                    break;
                case 3:
                    inserir_linha();
                    break;
                case 4:
                    limpar();
                    printf("----- LISTAR CONTEUDO -----\n");
                    listar();
                    printf("\n");
                    escolher_listagem();
                    break;
                case 5:
                    limpar();
                    printf("----- PESQUISAR VALOR -----\n");
                    listar();
                    printf("\n");
                    pesquisar_campo();
                    break;
                case 6:
                    limpar();
                    printf("----- APAGAR REGISTRO -----\n");
                    listar();
                    printf("\n");
                    apagar_registro();
                    break;
                case 7:
                    limpar();
                    printf("----- APAGAR TABELA -----\n");
                    listar();
                    printf("\n");
                    apagar_tabela();
                    break;
                case 8:
                    limpar();
                    printf("----- ATUALIZAR REGISTRO -----\n");
                    listar();
                    printf("\n");
                    atualizar_registro();
                    break;
                case 9:
                    limpar();
                    printf("----- INSTRUÇÕES -----\n");
                    printf("1)Digite um número de 1 a 8 para escolher a função desejada\n");
                    printf("\n2)Durante a execução da função escolhida, você terá que inserir informações de acordo com o requisitado\n");
                    printf("\n3)Certifique-se de inserir uma informação compatível com o requisitado e insira apenas o número de informações que será requisitado\n");
                    printf("\n4)A qualquer momento você pode digitar \"/cancelar\" para interromper a função atual\n");
                    printf("\n5)Esteja ciente que ao buscar informações em uma tabela, nós diferenciamos letras maiúsculas de minúsculas (letras maiúsculas tem valores menores que letras minúsculas)\n");
                    printf("\n6)Esteja ciente que ao criar ou pesquisar uma tabela, nós diferenciamos letras maiúsculas de minúsculas\n");
                    printf("\n7)Não insira acentos ou caracteres especiais em nenhum campo\n");
                    break;
                case 0:
                    limpar();
                    fim = 1;
                    printf("Até logo!\n");
                    break;
                default:
                    printf("Valor invalido!\n");
                    break;
            }
        }
        else{
            limpar();
            printf("Valor inválido\n");
        }
    }
    free(valor);
    return 0;
}