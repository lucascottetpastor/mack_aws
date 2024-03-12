#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];
    pid_t pid;
    char message[] = "Hello, child!";

    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe failed\n");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    }

    if (pid > 0) { // Processo pai
        char buffer[100];
        close(fd[1]); // Fecha a extremidade de escrita do pipe

        // Lê a mensagem do filho
        read(fd[0], buffer, sizeof(buffer));
        printf("Parent received message: %s\n", buffer);

        close(fd[0]); // Fecha a extremidade de leitura do pipe
    } else { // Processo filho
        close(fd[0]); // Fecha a extremidade de leitura do pipe

        // Escreve a mensagem para o pai
        write(fd[1], message, strlen(message)+1);
        close(fd[1]); // Fecha a extremidade de escrita do pipe
    }

    return 0;
}
