#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sched.h>

#define FIBER_STACK (1024 * 64)

// Estrutura de argumento para a thread filho
struct ThreadArgs {
    int value;
};

// Função que será executada pela nova thread
int threadFunction(void* argument) {
    struct ThreadArgs* args = (struct ThreadArgs*)argument;
    printf("Thread filho finalizando. Valor recebido: %d\n", args->value);
    return 0;
}

int main() {
    void* stack;
    pid_t pid;
    struct ThreadArgs args;

    // Aloca a pilha
    stack = malloc(FIBER_STACK);
    if (stack == 0) {
        perror("malloc: não foi possível alocar a pilha");
        exit(1);
    }

    printf("Criando thread filho\n");

    // Configuração dos argumentos para a thread filho
    args.value = 42;

    // Chama a chamada de sistema clone para criar a thread filho
    pid = clone(&threadFunction, (char*)stack + FIBER_STACK,
                SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &args);

    if (pid == -1) {
        perror("clone");
        exit(2);
    }

    // Aguarda a thread filho terminar
    pid = waitpid(pid, 0, 0);
    if (pid == -1) {
        perror("waitpid");
        exit(3);
    }

    // Libera a pilha
    free(stack);
    printf("Thread filho retornou e pilha liberada.\n");

    return 0;
}
