# Projeto 1 - Sistemas Operacionais
## Escada Rolante

#### 1. Qual a estratégia que você utilizou para evitar que duas pessoas acessem a escada rolante ao mesmo tempo em cada abordagem?
R: utilizei um mecanismo de exclusão mútua para garantir que apenas uma pessoa por vez possa modificar o estado compartilhado da escada rolante. No código usando threads, é utilizado um mutex, enquanto no código usando processos, é utilizado um semáforo para controlar o acesso da escada.

#### 2. Como garantir que somente uma das direções está ativa de cada vez em cada uma das abordagens?
R: Utilizei como método para garantir as direções fazendo uma verificação dentro da seção. As funções verificam se a direção atual é a mesma da pessoa que está tentando acessar a escada. Se for a mesma direção ou se a escada estiver parada, a pessoa pode acessar. Caso contrário, a direção da escada é alterada e um tempo é adicionado para a mudança de direção.

#### 3. Discorra sobre as diferenças entre as implementações utilizando threads e processos e diga qual foi mais eficiente na solução do problema, justificando sua resposta.
R: *Implementação utilizando threads:* cria-se várias threads que compartilham o mesmo espaço de memória, o que é mais eficiente em termos de consumo de recursos, uma vez que as threads compartilham recursos do processo pai. *Implementação utilizando processos:* cria-se processos separados, cada um com seu próprio espaço de endereço. Isso normalmente aumenta a sobrecarga, pois cada processo consome seus próprios recursos (memória).

## Execução com processo (E_1)

## Execução com thread (E_1)
