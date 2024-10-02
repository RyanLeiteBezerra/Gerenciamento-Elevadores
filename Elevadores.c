#include "Leitura.h"

void inicializar_variaveis(struct predio *predio, int q_andares, struct elevador **elevadores, int q_elevadores){

    predio->andares = malloc(q_andares*(sizeof(struct andar)));
    predio->numero_andares = q_andares;
    predio->numero_elevadores = q_elevadores;

    *elevadores = malloc(q_elevadores*(sizeof(struct elevador)));

    for(int i=0 ; i<q_elevadores ; i++)
    {
        (*elevadores+i)->p = NULL;
        (*elevadores+i)->estado = NULL;
        (*elevadores+i)->andar_atual = 0; //térreo
        (*elevadores+i)->demanda_atual = -1;
        (*elevadores+i)->movimentos = 0;
    }

    for(int i=0 ; i<q_andares ; i++){
        predio->andares[i].p = NULL;
        predio->andares[i].pessoas_saindo = 0;
    }
}

void inserir_final_estados(struct elevador *elevador, int andar, int instante){

    struct estado *aux = elevador->estado;
	while(aux != NULL && aux->prox != NULL) {
		aux = aux->prox;
	}

	struct estado *novo = malloc(sizeof(struct estado));
	novo->andar = andar;
    novo->instante = instante;
	novo->prox = NULL;

	if(aux == NULL)
		elevador->estado = novo;
	else {
		aux->prox = novo;
	}
}

struct pessoa* buscar(struct pessoa *p, int andar) {
    struct pessoa *aux = p;
    
    while(aux != NULL && aux->fim != andar) {
        aux = aux->prox;
    }
    
    return aux;
}

void excluir(struct pessoa **p, int andar){

    struct pessoa *aux = *p;
    struct pessoa *ant = *p;

    while(aux != NULL && aux->fim != andar) {
        ant = aux;
        aux = aux->prox;
    }
    
    if(*p == NULL){
    }
    else if(*p == aux){
        *p = aux->prox;
        free(aux);
    }
    else{
        ant->prox = aux->prox;
        free(aux);
    }
}

int tamanho_no(struct pessoa *p){

    int cont = 0;

    while(p != NULL){
        p = p->prox;
        cont++;
    }

    return cont;
}

int ja_e_demanda_atual(struct predio predio,struct elevador *elevadores, int andar){

    int diferenca;

    if(predio.numero_elevadores == 1)
        return 0;

    for(int i=0 ; i<predio.numero_elevadores; i++){

        if((elevadores+i)->demanda_atual != -1){

            diferenca = (elevadores+i)->demanda_atual - (elevadores+i)->andar_atual;

            if(diferenca > 0){

                if(andar >= (elevadores+i)->andar_atual)
                    return 1;
            }
            else if(diferenca < 0){

                if(andar <= (elevadores+i)->andar_atual)
                    return 1;
            }
            else
                return 1;
        }
    }
    
    return 0;
}

int indice_demanda_menos_distante(struct predio predio, struct elevador *elevadores, int indice_elevador, int andar_atual_elevador) {
    int indice_menor_distancia = -1; 
    int menor_distancia = predio.numero_andares;

    // Verificar demandas nos andares do prédio
    for (int i = 0; i < predio.numero_andares; i++) {
        if (predio.andares[i].p != NULL && ja_e_demanda_atual(predio,elevadores,i) == 0) {
            int distancia = abs(i - andar_atual_elevador);
            if (distancia < menor_distancia) { 
                menor_distancia = distancia;
                indice_menor_distancia = i;
            }
        }
    }

    // Verificar demandas das pessoas dentro do elevador
    struct pessoa *aux = (elevadores + indice_elevador)->p;
    while (aux != NULL) {
        int i = aux->fim; 
        int distancia = abs(i - andar_atual_elevador);
        if (distancia < menor_distancia) { // '<=' Pois, se a demanda de menor distancia estiver presente também no elevador, o elevador deve se movimentar!
            menor_distancia = distancia;    // Mesmo já sendo demanda de outro elevador.
            indice_menor_distancia = i;
        }
        aux = aux->prox;
    }

    return indice_menor_distancia;
}


void printar_predio(struct predio *predio, struct elevador *elevadores){

    for(int i=(predio->numero_andares-1) ; i>=0 ; i--){
        printf("A%.2d: ",i);

        for(int j=0 ; j<predio->numero_elevadores ; j++){
            if((elevadores+j)->andar_atual == i)
                printf("[%d]  ",tamanho_no((elevadores+j)->p));
            else
                printf("     ");
        }

        printf("| E = %d | S = %d |\n", tamanho_no(predio->andares[i].p), predio->andares[i].pessoas_saindo);
        predio->andares[i].pessoas_saindo = 0;
    }
}

void printar_estados_elevadores(struct predio *predio,struct elevador *elevadores){

    for(int i=0 ; i<predio->numero_elevadores ; i++){

        printf("\n");
        printf("E%d: ",(i+1));

        struct estado *estados = (elevadores+i)->estado;

        while(estados != NULL){

            printf("%d(%d) ",estados->andar,estados->instante);

            if(estados->prox != NULL)
                printf("=> ");
            else{
                printf("\n");
                printf("- Tempo de deslocamento do elevador %d: %ds\n", (i+1),estados->instante);
                printf("- Numero de andares percorrido pelo elevador %d: %d\n", (i+1),(elevadores+i)->movimentos);
            }

            estados = estados->prox;   
        }
    }

    printf("\n");

}

void tirar_colocar(struct predio *predio, struct elevador *elevadores){

    int andar_atual_elevador;

    for(int i=0 ; i<predio->numero_elevadores ; i++){

                andar_atual_elevador = (elevadores+i)->andar_atual;

                //Colocar pessoas do andar (i+1) no elevador:
                if(predio->andares[andar_atual_elevador].p != NULL)
                {
                    if((elevadores+i)->p == NULL){
                        (elevadores+i)->p = predio->andares[andar_atual_elevador].p;
                        predio->andares[andar_atual_elevador].p = NULL;
                    }
                    else{
                        struct pessoa *aux = (elevadores+i)->p;

                        while(aux != NULL && aux->prox != NULL) {
                            aux = aux->prox;
                        }

                        aux->prox = predio->andares[andar_atual_elevador].p;
                        predio->andares[andar_atual_elevador].p = NULL;
                    }
                }
            
                //Tirar pessoas cujo o andar fim seja o (i+1) do elevador:

                    while(buscar((elevadores+i)->p, andar_atual_elevador) != NULL){
                        excluir( &((elevadores+i)->p) , andar_atual_elevador);
                        predio->andares[andar_atual_elevador].pessoas_saindo++;
                    }

    }
}

void movimentar_elevador(struct predio *predio, struct elevador *elevadores, int tempo){
    int andar_atual_elevador, demanda_atual;

    for(int i=0 ; i< predio->numero_elevadores ; i++){

        andar_atual_elevador = (elevadores+i)->andar_atual;
        demanda_atual = (elevadores+i)->demanda_atual;

        inserir_final_estados((elevadores+i), (elevadores+i)->andar_atual , tempo);

        for(int j=0 ; j< predio->numero_elevadores ; j++){
            if(andar_atual_elevador == (elevadores+j)->demanda_atual){
                (elevadores+j)->demanda_atual = -1;
            }
        }

            demanda_atual = indice_demanda_menos_distante(*predio, elevadores, i, andar_atual_elevador);

            (elevadores+i)->demanda_atual = demanda_atual;

            if(demanda_atual != -1){

                if(andar_atual_elevador - demanda_atual > 0)
                    (elevadores+i)->andar_atual--;
                else
                    (elevadores+i)->andar_atual++;

                (elevadores+i)->movimentos++;
            }
    }
}

int movimentos_totais(struct predio *predio, struct elevador *elevadores){

    int soma = 0;

    for(int i=0 ; i<predio->numero_elevadores ; i++){
        soma += (elevadores+i)->movimentos;
    }

    return soma;
}

int checar_fim(struct predio *predio, struct elevador *elevadores){
    int andar_atual_elevador;
    int demanda;

    for(int i=0 ; i<predio->numero_elevadores ; i++){
        andar_atual_elevador = (elevadores+i)->andar_atual;
        demanda = indice_demanda_menos_distante(*predio, elevadores, i, andar_atual_elevador);
        if(demanda != -1)
            return 0;
    }

    return 1;
}

void iniciar_elevador(struct predio *predio, struct elevador *elevadores, struct fila_comandos *fila_comandos){

    int tempo=0;
    int comandos_lidos = 0;
    char leitura[2];

    printf("Digite algo para iniciar o elevador: ");
    scanf("%s",leitura);

    while(1){
        printf("Instante: %ds | Movimentos Totais: %d\n", tempo,movimentos_totais(predio,elevadores));

        comandos_lidos += processar_comandos(predio, elevadores, fila_comandos, tempo);

        tirar_colocar(predio, elevadores);

        printar_predio(predio, elevadores);

        movimentar_elevador(predio,elevadores,tempo);

        if (comandos_lidos == fila_comandos->tamanho && checar_fim(predio,elevadores))
            break;
        
        tempo++;

        sleep(1);
    }

    printar_estados_elevadores(predio,elevadores);
    printf("FIM: Tempo total = %ds | Movimentos Totais = %d\n", tempo,movimentos_totais(predio,elevadores));
    scanf("%s",leitura);

    liberar_fila_comandos(fila_comandos);
    free(elevadores);
    free(predio->andares);
}

