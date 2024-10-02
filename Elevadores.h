#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "Leitura.h"

// Declarações das funções
void inicializar_variaveis(struct predio *predio, int q_andares, struct elevador **elevadores, int q_elevadores);
void inserir_final_estados(struct elevador *elevador, int andar, int instante);
struct pessoa* buscar(struct pessoa *p, int andar);
void excluir(struct pessoa **p, int andar);
int tamanho_no(struct pessoa *p);
int ja_e_demanda_atual(struct predio predio,struct elevador *elevadores, int andar);
int indice_demanda_menos_distante(struct predio predio, struct elevador *elevadores, int indice_elevador, int andar_atual_elevador);
void printar_predio(struct predio *predio, struct elevador *elevadores);
void printar_estados_elevadores(struct predio *predio,struct elevador *elevadores);
void tirar_colocar(struct predio *predio, struct elevador *elevadores);
void movimentar_elevador(struct predio *predio, struct elevador *elevadores, int tempo);
int movimentos_totais(struct predio *predio, struct elevador *elevadores);
int checar_fim(struct predio *predio, struct elevador *elevadores);
void iniciar_elevador(struct predio *predio, struct elevador *elevadores, struct fila_comandos *fila_comandos);

#endif
