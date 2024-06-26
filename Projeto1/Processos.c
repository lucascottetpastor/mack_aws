#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>

#define MAX_PESSOAS 10000

typedef struct {
    int tempoChegada;
    int direcao;
} Passageiro;

typedef struct {
    int tempoFinal;
    int direcaoAtual; // -1 = escada parada
} EstadoEscada;

void funcaoPassageiro(Passageiro p, EstadoEscada *estado, sem_t *sem) {
    sleep(p.tempoChegada);
    sem_wait(sem);
    if (estado->direcaoAtual == -1 || estado->direcaoAtual == p.direcao) {
        estado->direcaoAtual = p.direcao;
        if (estado->tempoFinal < p.tempoChegada) {
            estado->tempoFinal = p.tempoChegada;
        }
        estado->tempoFinal += 10;
    } else {
        estado->tempoFinal += (estado->tempoFinal > p.tempoChegada) ? 10 : (p.tempoChegada - estado->tempoFinal) + 10;
        estado->direcaoAtual = p.direcao;
    }
    sem_post(sem);
}

int main() {
    int numPassageiros;
    scanf("%d", &numPassageiros);
    Passageiro passageiros[MAX_PESSOAS];

    // criação memória compartilhada
    int shm_fd = shm_open("/shmEstadoEscada", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(EstadoEscada));
    EstadoEscada *estado = (EstadoEscada *)mmap(NULL, sizeof(EstadoEscada), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    estado->tempoFinal = 0;
    estado->direcaoAtual = -1;

    // criação semáforo
    sem_t *sem = sem_open("/semEscada", O_CREAT, 0666, 1);

    // informações de cada pessoa
    for (int i = 0; i < numPassageiros; i++) {
        scanf("%d %d", &passageiros[i].tempoChegada, &passageiros[i].direcao);
    }

    // processo para cada pessoa
    for (int i = 0; i < numPassageiros; i++) {
        pid_t pid = fork();
        if (pid == 0) { // Processo filho
            funcaoPassageiro(passageiros[i], estado, sem);
            exit(0);
        } else if (pid < 0) {
            perror("fork");
            exit(1);
        }
    }

    // aguarda todos os processos filhos acabar
    while (wait(NULL) > 0);

    printf("O ultimo momento de parada é %d\n", estado->tempoFinal);

    // Limpeza
    munmap(estado, sizeof(EstadoEscada));
    close(shm_fd);
    shm_unlink("/shmEstadoEscada");
    sem_close(sem);
    sem_unlink("/semEscada");

    return 0;
}
