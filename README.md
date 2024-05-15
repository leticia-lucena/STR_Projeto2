# STR_Projeto2

# Executivo cíclico

## Descrição do Projeto
Este projeto visa desenvolver um sistema de executivo cíclico capaz de calcular os tempos de ciclo primário e secundário para um conjunto de tarefas, além de utilizar uma heurística para sugerir um possível escalonamento das tarefas dentro desses ciclos.

##Funcionalidades
- Cálculo dos tempos de ciclo: primeiramente, deve=-se obter a parametrização dos ciclos primário e secundário para execução das tarefas definidas. O cálculo é realizado por meio da análise de parâmetros das tarefas em questão.
- Cálculo do escalonamento usando heurísticas: a partir dos parâmetros de ciclo, é possível realizar o escalonamento utilizando dois métodos: Menor Tempo de Execução Primeiro (SETF) ou Maior Taxa de Periodicidade Primeiro (HRF).

## Algoritmo de Funcionamento
##Parte um: cálculo dos tempos de ciclo
- É definida uma structcom as informações de identidade, período e tempo de execução da tarefa. Também existe uma variável para armazenar a prioridade SETF, associando valores às tarefas de acordo com sua prioridade. Os valores são crescentes, sendo a maior prioridade igual a 1 e associada ao menor tempo de execução e assim por diante. Cria-se outra variável auxiliar para identificar a prioridade HRF, associando valores às tarefas de acordo com sua prioridade. Os valores são crescentes, sendo a maior prioridade igual a 1 e associada ao menor período e assim por diante.

A seguinte descrição pode ser utilizada como exemplo para implementar a caracterização das tarefas:

Dados:
T1: periodo 20, execucao 8
T2: periodo 20, execucao 7
T3: periodo 40, execucao 4
T4: periodo 40, execucao 3
T5: periodo 80, execucao 2

Prioridade calculadas para cada tarefa:
T1: setf 5, hrf 1
T2: setf 4, hrf 1
T3: setf 3, hrf 2
T4: setf 2, hrf 2
T5: setf 1, hrf 3

- Calcula-se o MMC e o MDC de todos os períodos das tarefas instanciadas, identificando o ciclo maior (primário) como o MMC calculado.
- O ciclo menor, também determinado de frame, possui tamanho que está no intervalo entre entre o maior tempo de execução dentre as tarefas e o menor período apresentados dentre as tarefas. Serão candidatos a tamanhos de frame todos os divisores do ciclo maior que estejam dentro desse intervalo. Para testar se o candidato é válido, utiliza-se 2*f - MDC(f,periodo da tarefa)<=periodo da tarefa, sendo f o candidato a tamanho de frame que está sendo testado.

##Parte dois: escalonamento
- Diretrizes: todas as tarefas devem ser executadas pelo menos uma vez no ciclo maior, mas não precisam estar em todos os frames. As tarefas serão alocadas seguindo uma heurística que otimize a execução das tarefas dentro do ciclo determinado. Otimizar, nesse caso, implica em alocar o máximo de tempo possível da forma mais dinâmica possível, diminuindo o tempo que o CPU passaria ocioso.
- Para a heurística de  menor tempo de execução primeiro (SETF), tarefas com menor tempo de execução são priorizadas. Ao executar rapidamente tarefas mais curtas, o sistema pode reduzir o tempo médio de espera  e aumentar a probabilidade de completar mais tarefas dentro de seus períodos. Sendo assim, tarefas com maior prioridade segundo o ranking realizado na primeira etapa serão alocadas primeiro e assim sucessivamente até que as diretrizes sejam cumpridas e não caibam mais tarefas nos frames. 
- Para a heurística de  maior taxa de periodicidade primeiro (HRF), as tarefas são priorizadas com base na frequência de sua execuçao, ou seja, tarefas com menor período devem ter prioridade. O procedimento realizado é similar à heurística descrita acima.
- Realiza-se o teste de alocação para as duas heurísticas utilizando as informações apresentadas, de acordo com seus tempos de execução, período e prioridades; e calcula-se o uso da CPU. A heurística escolhida deve ser a que apresenta maior taxa de uso da CPU, no entanto, seu uso não deve ultrapassar 100%, o que se configuraria como uma impossibilidade lógica.
- Por fim, o escalonamento apresentado para o usuário por meio de uma interface simples será apenas da heurística mais apropriada para as tarefas direcionadas, informando também o uso de CPU do escalonamento.

##Demonstração
O vídeo de demonstração do projeto pode ser encontrado no link: 
