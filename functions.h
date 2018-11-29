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
typedef struct {
    char nome[100];
} campo;
void alocar_arquivo(FILE** ptr, char *nome, char modo[]);
void apagar_registro();
void apagar_tabela();
void atualizar_registro();
void chamar_campos(char *nome, int qtdColunas);
int compara_string_proxima(char *a, char *b);
int contar_colunas(char *nome);
int contar_linhas(char *nome);
void criar_arquivo(char *nome);
void criar_coluna(coluna Coluna);
void criar_tabela();
campo escanear_campo(char *nome, int linha, int coluna);
void escrever_cabecalho(char *nome);
void escolher_listagem();
int espacos_por_coluna(char *nome, int qtdColunas, int posicao);
void inserir_linha();
void inserir_nome(char *nome);
void limpar();
void listar();
int listar_conteudo();
int listar_registro(char *nome, char *id);
void listar_estilo_linha(int qtdColunas, int *espacos);
void pesquisar_campo();
void pesquisar_registro(char *nome, int posicao, int tipo, int not_null);
void realizar_busca(char *nome, char *valor, int posicao, int tipo, int x);
void receber_campo(char *valor, int tipo, int not_null);
void receber_chave_primaria(char *valor);
void receber_nome_tabela(char *nome, int i);
void transcrever_colunas(FILE *leitura, FILE *escritaProvisoria, coluna *Coluna);
int ultimo_id_tabela(char *nome);
int verificar_chave(char *nome, char *valor);

#endif