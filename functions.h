#ifndef _FUNCTIONS_
#define _FUNCTIONS_

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <math.h>
typedef struct {
	int tipo, ai, not_null;
	char nome_coluna[100];
	char nome_tabela[100];
} coluna;
void alocar_arquivo(FILE** ptr, char *nome, char modo[]);
void apagar_registro();
void apagar_tabela();
void chamar_campos(char *nome, int qtd);
int checar_char(char *nome);
int checar_float(char *nome);
int checar_inteiro(char *nome);
int checar_chamada_campo(char *nome, int tipo);
int checar_nome_tabela(char *nome);
int converter_string_em_inteiro(char *nome);
void criar_arquivo(char *nome);
void criar_coluna(coluna Coluna);
void criar_tabela();
void inserir_linha();
void inserir_nome(char *nome);
int ler_tabela(char *nome);
void listar();
void listar_conteudo();
void pesquisar_campo();
void pesquisar_registro(char *nome, int posicao, int tipo);
void realizar_busca(char *nome, char *valor, int posicao, int tipo, int x);
void receber_nome_tabela(char *nome, int i);
int ultimo_id_tabela(char *nome);
int verificar_chave(char *nome, char *valor);

#endif