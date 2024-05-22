## Objetivos do Projeto:

Explorar a arquitetura e operações de um sistema operacional.
Tratar questões de concorrência no contexto de sistemas operacionais.
Implementar um software que faça uso eficiente das chamadas de sistema disponíveis no SO.
Fomentar o entendimento sobre comunicação entre processos em um ambiente controlado.
## Requisitos Técnicos:

O software deve ser compilável e executável em sistemas Linux.
O código deve ser hospedado em um repositório público de fácil acesso, sem necessidade de autenticação.
Deve ser independente de quaisquer bibliotecas ou dependências comerciais.
## Documentação do Código:

Estrutura Principal do Código:

Mutex:

Declaração: pthread_mutex_t mutexTransferencia;
Utilizado para controlar o acesso às contas durante operações de transferência.
## Inicialização e Finalização do Mutex:

Inicialização: pthread_mutex_init(&mutexTransferencia, NULL);
Destruição: pthread_mutex_destroy(&mutexTransferencia);
O mutex é inicializado no começo do programa e destruído ao final para assegurar a correta liberação dos recursos.
## Função de Operação de Transferência:

Nome da Função: void *executarTransacao(void *parametros)
Executada por threads para realizar a transferência de fundos, com verificação de saldo adequado.
Emprega pthread_mutex_lock(&mutexTransferencia); e pthread_mutex_unlock(&mutexTransferencia); para sincronizar o acesso às contas.
Explicação Detalhada da Implementação:

## Gerenciamento do Mutex:

Um mutex é declarado e gerido para evitar acessos simultâneos às contas, essencial para a integridade das transações.
## Função de Execução de Transações:

A função executarTransacao gerencia as operações de transferência, bloqueando o acesso às contas durante a transação através do mutex.
## Criação e Gerenciamento de Threads:

Threads são criadas utilizando pthread_create para permitir múltiplas transações simultâneas.
Um loop adicional garante que todas as threads sejam sincronizadas ao final das operações, aguardando o término de cada uma antes de proceder com a liberação de recursos.
## Pré-requisitos para Execução:

Compilador GCC.
Sistema Operacional Linux.
