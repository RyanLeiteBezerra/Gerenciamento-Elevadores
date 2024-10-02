#ifndef LEITURA_H
#define LEITURA_H

#include "Variaveis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Declarações de funções

struct comando* criar_novo_comando(char *comando_str);

void inserir_comando(struct fila_comandos *fila, char *comando_str);

void printar_fila(struct fila_comandos *fila);

void liberar_fila_comandos(struct fila_comandos *fila);

int* processar_string_e_inserir_na_fila(char *comandos, struct fila_comandos *fila);

void printar_fila_comandos(struct fila_comandos *fila);

void inserir_inicio(struct pessoa **p, struct pessoa dados);

int atualizar_estado_elevadores(struct elevador *elevadores, struct fila_comandos *fila_comandos);

int processar_comandos_passageiros(struct predio *predio, struct fila_comandos *fila_comandos, int instante);

int processar_comandos(struct predio *predio, struct elevador *elevadores, struct fila_comandos *fila_comandos, int instante);

char* ler_string();

#endif
