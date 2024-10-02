#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct pessoa{
    int fim;
    struct pessoa *prox;
};

struct andar{
    int pessoas_saindo;
    struct pessoa *p; 
};

struct predio{
    struct andar *andares;
    int numero_andares;
    int numero_elevadores;
};

struct elevador{
    int andar_atual;
    int demanda_atual;
    int movimentos;
    struct pessoa *p;
    struct estado *estado;
};

struct estado{
    int andar;
    int instante;
    struct estado *prox;
};

struct comando {
    char comando[50];
    struct comando *prox;
};

struct fila_comandos {
    struct comando *inicio; 
    int tamanho;
};