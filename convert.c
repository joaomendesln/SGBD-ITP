#include "math.h"
#include "convert.h"

//converte uma string em double
double converter_string_em_double(char *nome){
    int tamanho = strlen(nome), p = (int)nome[0], c, posicao = 0, qtdDecimais = 0;
    double cont = 0, numero = 0, decimal = 0;
    for (int i = 0; i < tamanho; i++){
        c = (int)nome[i];
        if (c == 46) {
            posicao = i;
            break;
        }
    }

    posicao++;
    if(p != 45){
        for (int i = posicao - 2; i >= 0; i--){
            int x = (int)nome[i] % 48;
            numero += x * pow(10,cont);
            cont++;
        }
    }
    else{
        for (int i = posicao - 2; i > 0; i--){
            int x = (int)nome[i] % 48;
            numero += x * pow(10,cont);
            cont++;
        }
        numero *= -1;
    }
    cont = 0;
    for (int i = tamanho - 1; i >= posicao; i--){
        int x = (int)nome[i] % 48;
        decimal += x * pow(10,cont);
        cont++;
    }
    decimal = decimal / pow(10,cont);
    if (p == 45) decimal *= -1;
    return (numero + decimal);
}
//converte uma string em inteiro
int converter_string_em_inteiro(char *nome){
    int tamanho = strlen(nome), numero = 0, sinal = 1, p = (int)nome[0];
    float cont = 0;
    if(p != 45){
        for (int i = tamanho - 1; i >= 0; i--){
            int x = (int)nome[i] % 48;
            numero += x * pow(10,cont);
            cont++;
        }
    }
    else{
        for (int i = tamanho - 1; i > 0; i--){
            int x = (int)nome[i] % 48;
            numero += x * pow(10,cont);
            cont++;
        }
        numero *= -1;
    }
    return numero;
}