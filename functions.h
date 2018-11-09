#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
struct campo{
	int tipo, ai, not_null, cp;
	char nome_campo[100];
};
void ler_nome_tabela(char nome_arquivo[100]);
int checar_nome_tabela(char nome[100]);
void listar();
void criar_arquivo(char nome[100]);
void criar_campo(char nome[100], int cp, int tipo, int ai, int not_null, char campo[100]);
void criar_tabela();
void inserir_linha();