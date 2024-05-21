#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Tarefa
{
    char id[10];
    int periodo;
    int tempo_execucao;
    int prioridade;
};


//PARTE I - Cálculo dos Tempos de Ciclo

// Cálculo do MDC
int mdc(int a, int b)
{
    if (b == 0)
        return a;
    return mdc(b, a % b);
}

//Cálculo do MMC
int mmc(int a, int b)
{
    return (a * b) / mdc(a, b);
}

// Cálculo do tamanho de cada ciclo (primário e secundário)
void calcularCiclos(struct Tarefa tarefas[], int n, int *ciclo_primario, int *ciclo_secundario)
{
    //Define o primeiro valor para ciclo como o valor de período da primeira tarefa
    *ciclo_primario = tarefas[0].periodo;
    *ciclo_secundario = tarefas[0].periodo;
    
    //Teste de critério: define que o tamanho do ciclo obedecerá a regra de MMC e MDC
    for (int i = 1; i < n; i++)
    {
        *ciclo_primario = mmc(*ciclo_primario, tarefas[i].periodo);
        *ciclo_secundario = mdc(*ciclo_secundario, tarefas[i].periodo);
    }
}

//Ordenação das tarefas com base no tamanho do período (HRF)
int compararPeriodo(const void *a, const void *b)
{
    const struct Tarefa *tarefaA = (const struct Tarefa *)a;
    const struct Tarefa *tarefaB = (const struct Tarefa *)b;
    return tarefaA->periodo - tarefaB->periodo;
}

//Ordenação das tarefas com base no tempo de execução (SETF)
int compararExecucao(const void *a, const void *b)
{
    const struct Tarefa *tarefaA = (const struct Tarefa *)a;
    const struct Tarefa *tarefaB = (const struct Tarefa *)b;
    return tarefaA->tempo_execucao - tarefaB->tempo_execucao;
}

//PARTE II - Escalonamento

//Verificação de resquisitos
int verificarRequisitos(struct Tarefa tarefas[], int num_tarefas, int frame_size)
{
    // Ordena as tarefas por período em ordem crescente
    qsort(tarefas, num_tarefas, sizeof(struct Tarefa), compararPeriodo);
    int i, j;
    for (i = 0; i < num_tarefas; i++)
    {
        // Requisito #1
        if (frame_size < tarefas[i].tempo_execucao)
        {
            return 0; // Requisito #1 não cumprido
        }

        // Requisito #2
        if (tarefas[i].periodo % frame_size != 0)
        {
            return 0; // Requisito #2 não cumprido
        }

        // Requisito #3
        if (2 * frame_size - mdc(tarefas[i].periodo, frame_size) > tarefas[i].periodo)
        {
            return 0; // Requisito #3 não cumprido
        }
    }
    return 1; // Todos os requisitos atendidos
}
int encontrarFrame(struct Tarefa tarefas[], int num_tarefas, int ciclo_primario)
{
    // Ordena as tarefas por período em ordem crescente
    qsort(tarefas, num_tarefas, sizeof(struct Tarefa), compararPeriodo);
    // variavel pra armazenar o maior frame size
    int frame_size = 0;

    for (int j = 1; j <= ciclo_primario; j++)
    {
        if (ciclo_primario % j == 0)// Requisito #2
        {
            // verifica se pra esse divisor do ciclo primario todos os requisitos são aceitos
            for (int i = 0; i < num_tarefas; i++)
            {
                // Requisito #1
                if (j > tarefas[i].tempo_execucao)
                {
                    // Requisito #2.5
                    if ((tarefas[i].periodo % j) == 0)
                    {

                        // Requisito #3
                        if (2 * j - mdc(tarefas[i].periodo, j) < tarefas[i].periodo)
                        {
                            return frame_size;
                        }


                    }
                }
            }
        }

    }
}
void dividirTarefasHRF(struct Tarefa tarefas[], int numTarefas, int ciclo_primario, int ciclo_secundario)
{
    // Ordena as tarefas por período em ordem crescente
    qsort(tarefas, numTarefas, sizeof(struct Tarefa), compararPeriodo);

    // Array que controla se cada tarefa já foi executada em um ciclo secundário
    int *tarefaExecutada = (int *)calloc(numTarefas, sizeof(int));

    // Quando tarefa é executada pela sobra de tempo, ocorre uma atualização do periodo,
    // Assim, é criada uma variável auxiliar para modificar ou não o periodo
    int *novPeriodo = (int*)calloc(numTarefas, sizeof(int));

    // Preenche a variável auxiliar do período
    for (int j = 0; j < numTarefas; j++)
    {
        novPeriodo[j] = tarefas[j].periodo;
    }

    // Divide as tarefas em ciclos usando o ciclo secundário
    printf("\nEscalonamento Sugerido (Heurística: Highest Rate First - HRF):\n");
    printf("----------------------------------------------------------------------\n");
    int tempo_atual = 0;
    int ciclo_atual = 1;

    //Laço de execução para cada ciclo menor
    for (int ciclo = 1; ciclo <= ciclo_primario / ciclo_secundario; ciclo++)
    {
        printf("Ciclo Secundário %d:\n", ciclo);
        int ciclo_restante = ciclo_secundario;

        for (int i = 0; i < numTarefas; i++)
        {
            // Para cada tarefa, verifica se está no período de execução e se há tempo de processamento suficiente
            if (tempo_atual % novPeriodo[i] == 0)
            {
                if (tarefas[i].tempo_execucao <= ciclo_restante)
                {
                    printf("  - %s: tempo de execucao = %d, periodo = %d, prioridade = %d\n",
                           tarefas[i].id, tarefas[i].tempo_execucao, tarefas[i].periodo, tarefas[i].prioridade);
                    tarefaExecutada[i] = 1; // Marca a tarefa como executada
                    ciclo_restante -= tarefas[i].tempo_execucao;
                }
            }
            // Para cada tarefa, verifica se há tempo hábil para execução
            else if (!tarefaExecutada[i] && tarefas[i].tempo_execucao <= ciclo_restante)
            {
                printf("  - %s: tempo de execucao = %d, periodo = %d, prioridade = %d (executada no tempo restante)\n",
                tarefas[i].id, tarefas[i].tempo_execucao, tarefas[i].periodo, tarefas[i].prioridade);
                tarefaExecutada[i] = 1;                    // Marca a tarefa como executada
                ciclo_restante -= tarefas[i].tempo_execucao; // Reduz o tempo restante no ciclo
                novPeriodo[i] = tarefas[i].periodo + (ciclo - 1) * ciclo_secundario;
            }
        }

        // Atualiza o tempo e ciclo atual
        tempo_atual += ciclo_secundario;
        ciclo_atual++;
    }
}

// Função que calcula a viabilidade de escalonamento com base na utilização das tarefas definidas
float calcularTaxaUtilizacao(struct Tarefa tarefas[], int numTarefas)
{
    // Taxa de utilização total
    float utilizacao_total = 0.0;

    // Calcula a taxa de utilização de cada tarefa e adiciona à utilização total
    for (int i = 0; i < numTarefas; i++)
    {
        float utilizacao_tarefa = (float)tarefas[i].tempo_execucao / (float)tarefas[i].periodo;
        utilizacao_total += utilizacao_tarefa;
    }

    // Imprime a taxa de utilização total do sistema
    printf("Verificação de Escalonabilidade:\n");
    printf("--------------------------------\n");
    printf("Utilizacao: %.2f\n", utilizacao_total);

    return utilizacao_total;
}

int main()
{
  
  //Declaração das tarefas
  
struct Tarefa tarefas[] = {
    {"Tarefa1", 14, 1, 1},
    {"Tarefa2", 20, 2, 2},
    {"Tarefa3", 22, 3, 1}
};

  
  int numTarefas = sizeof(tarefas) / sizeof(tarefas[0]);

    // Cálculo da taxa de utilização total e verificação de viabilidade
    float utilizacao_total = calcularTaxaUtilizacao(tarefas, numTarefas);
  
    // Caso a utilização > 1, o escalonamento não é viável, e os calculos são abortados
    if (utilizacao_total > 1.0)
    {
        printf("Escalonamento nao viavel.\n\n");
      
        return 1;
    }
    else
    {
        printf("Escalonamento viavel.\n\n");
      
        // Cálculo dos ciclos primário e secundário
        int ciclo_primario, ciclo_secundario;
        calcularCiclos(tarefas, numTarefas, &ciclo_primario, &ciclo_secundario);
      
        // Verificar se os requisitos são atendidos
        // Imprime a taxa de utilização total do sistema
        printf("Escolha do Frame Verificando os Requisitos:\n");
        printf("-------------------------------------------\n");
        if (verificarRequisitos( tarefas,  numTarefas, ciclo_secundario) > 0)
        {
            printf("Tamanho do frame: %d\n", ciclo_secundario);
            printf("Requisitos cumpridos \n\n");
            printf("-------------------------------------------\n");
            // Impressão dos ciclos calculados
            printf("Calculo de Ciclos para o Executivo Ciclico:\n");
            printf("-------------------------------------------\n");
            printf("Tempo de Ciclo Primario: %d unidades de tempo\n", ciclo_primario);
            printf("Tempo de Ciclo Secundario: %d unidades de tempo\n", ciclo_secundario);

            // Divisão das tarefas em ciclos
            dividirTarefasHRF(tarefas, numTarefas, ciclo_primario, ciclo_secundario);


            return 0;
        }
        else // como o ciclo secundario pelo mdc n atende requisitos procurar um q atenda
        {
            ciclo_secundario = encontrarFrame(tarefas, numTarefas, ciclo_primario);
            if (ciclo_secundario > 0)
            {
                printf("Tamanho do frame: %d\n", ciclo_secundario);
                printf("Requisitos cumpridos \n\n");
                printf("-------------------------------------------\n");
                // Impressão dos ciclos calculados
                printf("Calculo de Ciclos para o Executivo Ciclico:\n");
                printf("-------------------------------------------\n");
                printf("Tempo de Ciclo Primario: %d unidades de tempo\n", ciclo_primario);
                printf("Tempo de Ciclo Secundario: %d unidades de tempo\n", ciclo_secundario);

                // Divisão das tarefas em ciclos
                dividirTarefasHRF(tarefas, numTarefas, ciclo_primario, ciclo_secundario);

                return 0;
            }
            else
            {
                printf("Os requisitos não estão sendo cumpridos para nenhum frame \n");
            }
        }
    }

    return 0;
}
