#include <stdio.h>
#include <stdlib.h>
#include "Elevadores.h"

void main() {
    struct predio predio;
    struct elevador *elevadores;
    struct fila_comandos fila_comandos;
    char *string;
    int *elevores_andares;

    //"AM_25 E1_04_S_6,9,8 E2_11_D_5,8,9,3,2,T E3_20_D_5,8,T,9 T5_P01_S_4_6 T9_P02_S_5_10 T10_P03_S_3_9 T10_P04_D_6_T T10_P05_S_8_15 T12_P01_D_6_4 T15_P06_D_9_2 T15_P07_S_2_13 T18_P08_D_8_T T21_P01_D_16_3 T21_P10_S_T_13 T21_P11_S_T_12 T23_P12_S_T_15 T28_P13_S_2_13";
    string = ler_string();
    elevores_andares = processar_string_e_inserir_na_fila(string, &fila_comandos);

    inicializar_variaveis(&predio, *(elevores_andares+1), &elevadores, *(elevores_andares+0));

    iniciar_elevador(&predio, elevadores, &fila_comandos);
}
