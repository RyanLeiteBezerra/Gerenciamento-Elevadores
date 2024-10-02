# Gerenciamento-Elevadores

Durante o curso de Estrutura de Dados no IFCE, desenvolvi um sistema de simulação de elevadores que implementa conceitos fundamentais de organização e manipulação de dados em C. O sistema gerencia múltiplos elevadores, permitindo processar comandos, alocar recursos de memória e registrar movimentos dos elevadores em um prédio com vários andares e demandas.

Principais funcionalidades:

- Inicialização dinâmica de prédios e elevadores utilizando estruturas de dados como filas e listas encadeadas.
- Processamento eficiente de comandos e demandas dos elevadores.
- Simulação do movimento dos elevadores e atendimento de requisições de andares.
- Manipulação de filas para gerenciar o fluxo de pessoas dentro dos elevadores.

Ferramentas e técnicas utilizadas:

- Alocação dinâmica de memória com malloc e estruturas encadeadas.
- Estruturas de controle como fila de comandos para gerenciar o fluxo de entrada.
- Funções separadas em arquivos .h e .c para modularidade e reutilização de código.

Formato dos comandos:

AM_25 E1_04_S_6,9,8 E2_11_D_5,8,9,3,2,T E3_20_D_5,8,T,9 T5_P01_S_4_6
T8_P01_D_6_4 T9_P02_S_5_10 T10_P03_S_3_9 T10_P04_D_6_T T10_P05_S_8_15
T15_P06_D_9_2 T15_P07_S_2_13 T18_P08_D_8_T T21_P01_D_16_3 T21_P10_S_T_13
T21_P11_S_T_12 T23_P12_S_T_15 T28_P13_S_2_13

AM_25 signifca que o andar máximo é o 25o.

E1_04_S_6,9,8 significa que o elevador está no 4o andar, subindo e que as pessoas no
elevador tinham apertado os andares 6, 9 e 8.

T5_P01_S_4_6 significa que a pessoa 1 no tempo 5 segundos estava no 4o andar e
quando ela entrar no elevador ela irá apertar o botão para ir ao 6o andar.
