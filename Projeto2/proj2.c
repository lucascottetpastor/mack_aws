#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Conta {
    char nome[10];
    int saldo;
};
typedef struct Conta Conta;

struct Transacao {
    Conta *destinatario;
    Conta *remetente;
    int valor;
    int *contadorSucesso;
};
typedef struct Transacao Transacao;

Conta contaA;
Conta contaB;
pthread_mutex_t trava;

void *processarTransacao(void *arg) {
    Transacao *transacao = (Transacao *)arg;
    pthread_mutex_lock(&trava);

    printf("---------------------------------------\n");
    if (transacao->remetente->saldo - transacao->valor >= 0) {
        printf("Transação de %s para %s\n", transacao->remetente->nome, transacao->destinatario->nome);
        printf("Valor da transação: R$%d,00\n", transacao->valor);

        printf("%s - Saldo Anterior: R$%d,00\n", transacao->remetente->nome, transacao->remetente->saldo);
        transacao->remetente->saldo -= transacao->valor;
        printf("%s - Saldo Atual: R$%d,00\n", transacao->remetente->nome, transacao->remetente->saldo);

        printf("%s - Saldo Anterior: R$%d,00\n", transacao->destinatario->nome, transacao->destinatario->saldo);
        transacao->destinatario->saldo += transacao->valor;
        printf("%s - Saldo Atual: R$%d,00\n", transacao->destinatario->nome, transacao->destinatario->saldo);

        (*transacao->contadorSucesso)++;
    } else {
        printf("Saldo Insuficiente\n");
    }

    pthread_mutex_unlock(&trava);
    free(transacao);
    return NULL;
}

int main(void) {
    srand(time(NULL));
    strcpy(contaA.nome, "Conta A");
    contaA.saldo = 1000;
    strcpy(contaB.nome, "Conta B");
    contaB.saldo = 1000;

    pthread_mutex_init(&trava, NULL);

    int totalTransacoes = 10;
    int transacoesSucesso = 0;
    int iteracoes = totalTransacoes / 100 + (totalTransacoes % 100 != 0);
    int indice = 0;

    for (int i = 0; i < iteracoes; i++) {
        pthread_t threads[100];

        for (int j = 0; j < 100; j++) {
            if (indice >= totalTransacoes) {
                break;
            }

            Transacao *t = (Transacao *)malloc(sizeof(Transacao));
            int escolha = rand() % 2;
            if (escolha == 0) {
                t->remetente = &contaA;
                t->destinatario = &contaB;
            } else {
                t->remetente = &contaB;
                t->destinatario = &contaA;
            }
            t->valor = rand() % 500 + 1;
            t->contadorSucesso = &transacoesSucesso;

            pthread_create(&threads[j], NULL, processarTransacao, (void *)t);
            indice++;
        }

        for (int j = 0; j < 100; j++) {
            if (j < indice) {
                pthread_join(threads[j], NULL);
            }
        }
    }

    pthread_mutex_destroy(&trava);

    printf("---------------------------------------\n");
    printf("Total de Transações Tentadas: %d\n", totalTransacoes);
    printf("Total de Transações Bem-sucedidas: %d\n", transacoesSucesso);
    printf("Saldo final da %s: R$%d,00\n", contaA.nome, contaA.saldo);
    printf("Saldo final da %s: R$%d,00\n", contaB.nome, contaB.saldo);

    return 0;
}
