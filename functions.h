#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
typedef struct {
	int tipo, ai, not_null;
	char nome_coluna[100];
	char nome_tabela[100];
} coluna;
void ler_nome_tabela(char nome_arquivo[100]);
int checar_nome_tabela(char nome[100]);
void listar();
void criar_arquivo(char nome[100]);
void inserir_nome(char nome[100]);
void criar_coluna(coluna Coluna);
void criar_tabela();
void inserir_linha();
int ultimo_id_tabela(char nome[100]);
void listar_conteudo();
void pesquisar_campo();