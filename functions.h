#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
typedef struct {
	int tipo, ai, not_null;
	char nome_coluna[100];
	char nome_tabela[100];
} coluna;
void alocar_arquivo(FILE** ptr, char *nome, char modo[]);
void chamar_campos(char *nome, int qtd);
int checar_nome_tabela(char *nome);
void criar_arquivo(char *nome);
void criar_coluna(coluna Coluna);
void criar_tabela();
void inserir_linha();
void inserir_nome(char *nome);
int ler_tabela(char *nome);
void listar();
void listar_conteudo();
void pesquisar_campo();
void pesquisar_registro(char *nome, int posicao);
void receber_nome_tabela(char *nome, int i);
int ultimo_id_tabela(char *nome);
int verificar_chave(char *nome, char *valor);