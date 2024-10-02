#include "Variaveis.h"

struct comando* criar_novo_comando(char *comando_str) {
    struct comando *novo = malloc(sizeof(struct comando));
    strcpy(novo->comando, comando_str);  
    novo->prox = NULL;  
    return novo;
}

void inserir_comando(struct fila_comandos *fila, char *comando_str) {
    struct comando *novo = criar_novo_comando(comando_str); 
    
    if (fila->inicio == NULL) {
        fila->inicio = novo;  
    } else {
        struct comando *temp = fila->inicio;
        while (temp->prox != NULL) {
            temp = temp->prox; 
        }
        temp->prox = novo;
    }
    
    fila->tamanho++;
}

void printar_fila(struct fila_comandos *fila) {
    struct comando *temp = fila->inicio;
    while (temp != NULL) {
        printf("Comando: %s\n", temp->comando);
        temp = temp->prox;
    }
    printf("Tamanho da fila: %d\n", fila->tamanho);
}

void liberar_fila_comandos(struct fila_comandos *fila) {
    struct comando *atual = fila->inicio;
    struct comando *proximo;

    while (atual != NULL) {
        proximo = atual->prox;  
        free(atual);            
        atual = proximo;       
    }

    fila->inicio = NULL;
    fila->tamanho = 0;
}

void printar_fila_comandos(struct fila_comandos *fila);

int* processar_string_e_inserir_na_fila(char *comandos, struct fila_comandos *fila) {

    fila->inicio = NULL;
    fila->tamanho = 0;

    char *token = strtok(comandos, " ");
    int *quantElevadores_quantAndares = malloc(2 * sizeof(int));
    quantElevadores_quantAndares[0] = 0;  // Quantidade de elevadores
    quantElevadores_quantAndares[1] = 0;  // Quantidade de andares
    int aux;

    while (token != NULL) {
        if (token[0] == 'A') {
            sscanf(token, "AM_%d", &aux);
            quantElevadores_quantAndares[1] = aux + 1;  // Atualiza o número de andares (andar + 1)
        }
        else if (token[0] == 'E') {
            sscanf(token, "E%d_%*[^_]", &aux);  // Extrai o número do elevador

            if (aux > quantElevadores_quantAndares[0]) {
                quantElevadores_quantAndares[0] = aux;  // Atualiza o número máximo de elevadores
            }
        }

        inserir_comando(fila, token);  // Insere cada comando na fila
        token = strtok(NULL, " ");  // Próximo comando
    }

    printar_fila_comandos(fila);
    free(comandos);
    return quantElevadores_quantAndares;
}

void printar_fila_comandos(struct fila_comandos *fila){

    struct comando *aux = fila->inicio;

    printf("\n");
    
    while(aux != NULL){

        printf("%s\n",aux->comando);

        aux = aux->prox;
    }
}

void inserir_inicio(struct pessoa **p, struct pessoa dados){

	struct pessoa *novo = malloc(sizeof(struct pessoa));
	*novo = dados;
    novo->prox = *p;
	*p = novo;
}

int atualizar_estado_elevadores(struct elevador *elevadores, struct fila_comandos *fila_comandos) {
    int elevador_num, comandos_lidos = 0;
    char andar_atual_str[10]; // Para armazenar o andar como string (incluindo "T")
    char destinos[50]; // Aumentar o tamanho para comportar destinos múltiplos

    struct comando *comandos = fila_comandos->inicio;

    while(comandos != NULL){
        if(comandos->comando[0] == 'A')
            comandos_lidos++;
        else if(comandos->comando[0] == 'E'){

            comandos_lidos++;
        
            sscanf(comandos->comando, "E%d_%[^_]_%*[^_]_%s", &elevador_num, andar_atual_str, destinos);
            // Checar se o andar atual é "T" (térreo) ou um número
            int andar_atual = (strcmp(andar_atual_str, "T") == 0) ? 0 : atoi(andar_atual_str);
            
            // Atualizar o estado do elevador com as informações obtidas
            elevadores[elevador_num - 1].andar_atual = andar_atual;

            // Processar destinos separados por vírgulas
            char *token = strtok(destinos, ",");
            while (token != NULL) {
                struct pessoa nova_pessoa;
                if (strcmp(token, "T") == 0) {
                    nova_pessoa.fim = 0;  // Térreo
                } else {
                    nova_pessoa.fim = atoi(token);
                }

                // Inserir o passageiro na lista do elevador
                inserir_inicio(&(elevadores[elevador_num - 1].p), nova_pessoa);

                // Próximo destino
                token = strtok(NULL, ",");
            }
        }

        comandos = comandos->prox;
    }
    
    return comandos_lidos;
}

int processar_comandos_passageiros(struct predio *predio, struct fila_comandos *fila_comandos,int instante) {

    int tempo, fim, comando_lidos = 0;
    char inicio_str[10]; // Para armazenar o andar de início como string (incluindo "T")
    char final[30]; // Para armazenar o andar de destino

    struct comando *comandos = fila_comandos->inicio;

    while(comandos != NULL){

        if(comandos->comando[0] == 'T'){

            // Parse da string para extrair informações da demanda do passageiro, ignorando direção e ID
            sscanf(comandos->comando, "T%d_%*[^_]_%*[^_]_%[^_]_%s", &tempo, inicio_str, final);

            // Verificar se o comando corresponde ao instante atual
            if (tempo == instante) {
                
                comando_lidos++;

                // Checar se o andar de início é "T" (térreo) ou um número
                int inicio = (strcmp(inicio_str, "T") == 0) ? 0 : atoi(inicio_str);

                // Checar se o andar de destino é "T" (térreo) ou um número
                if (strcmp(final, "T") == 0) {
                    fim = 0;  // Se o destino for o térreo
                } else {
                    fim = atoi(final);
                }

                // Cria uma nova pessoa e insere na lista do andar de origem
                struct pessoa nova_pessoa;
                nova_pessoa.fim = fim;

                inserir_inicio(&(predio->andares[inicio].p), nova_pessoa);
            }
        }

        comandos = comandos->prox;

    }

    return comando_lidos;
}

int processar_comandos(struct predio *predio, struct elevador *elevadores, struct fila_comandos *fila_comandos, int instante) {
    // Variável estática para rastrear se atualizar_estado_elevadores já foi chamada
    static int primeira_chamada = 1;
    int comandos_lidos = 0;

    if (primeira_chamada) {
        comandos_lidos += atualizar_estado_elevadores(elevadores, fila_comandos);
        primeira_chamada = 0; // Define para 0 para que não seja executada nas chamadas futuras
    }

    comandos_lidos += processar_comandos_passageiros(predio, fila_comandos, instante);

    return comandos_lidos;
}

char* ler_string() {
    char *string = malloc(1024 * sizeof(char));  
    if (string == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);  
    }

    printf("Digite os comandos: ");
    if (fgets(string, 1024, stdin) != NULL) {
        return string;  
    } else {
        free(string);   
        return NULL;    
    }
}
