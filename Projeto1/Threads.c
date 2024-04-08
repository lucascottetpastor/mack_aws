#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_PESSOAS 10000

// armazenar informações das pessoas
typedef struct {
    int tempoChegada;
    int direcao;
} Passageiro;

// compartilhamento
int tempoFinal = 0;
int direcaoAtual = -1; // -1 indica que a escada está parada
pthread_mutex_t mutex;

// função executada por cada thread
void* funcaoPassageiro(void* arg) {
    Passageiro* p = (Passageiro*)arg;

    // simulação do tempo
    sleep(p->tempoChegada);

    pthread_mutex_lock(&mutex);
    // verificação da direção e ajuste do tempo final
    if (direcaoAtual == -1 || direcaoAtual == p->direcao) {
        direcaoAtual = p->direcao;
        if (tempoFinal < p->tempoChegada) {
            tempoFinal = p->tempoChegada;
        }
        tempoFinal += 10;
    } else {
        // A escada precisa mudar de direção
        tempoFinal += (tempoFinal > p->tempoChegada) ? 10 : (p->tempoChegada - tempoFinal) + 10;
        direcaoAtual = p->direcao;
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    int numPassageiros;
    scanf("%d", &numPassageiros);

    Passageiro passageiros[MAX_PESSOAS];
    pthread_t threads[MAX_PESSOAS];
    pthread_mutex_init(&mutex, NULL);

    // informações de cada pessoa
    for (int i = 0; i < numPassageiros; i++) {
        scanf("%d %d", &passageiros[i].tempoChegada, &passageiros[i].direcao);
    }

    // criação de thread para cada pessoa
    for (int i = 0; i < numPassageiros; i++) {
        pthread_create(&threads[i], NULL, funcaoPassageiro, (void*)&passageiros[i]);
    }

    // termino de todas threads
    for (int i = 0; i < numPassageiros; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("O ultimo momento de parada é %d\n", tempoFinal);

    pthread_mutex_destroy(&mutex);
    return 0;
}
