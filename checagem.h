#ifndef _CHECAGEM_
#define _CHECAGEM_

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <math.h>

int checar_char(char *nome);
int checar_float(char *nome);
int checar_inteiro(char *nome);
int checar_chamada_campo(char *nome, int tipo);
int checar_nome_tabela(char *nome);

#endif