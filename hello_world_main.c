/* GPIO Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

/*

conceitos freertos:
 funções e variaveis iniciam com o tipo ex:
 vTaskPrioritySet() - retorna void
 xQueueReceive() retorna BaseType_t
 pvTimerGetTimerID() - retorna ponteiro para void
 variaveis: c - char s - short l - int32_t u - unsigned p - pointer

 macros:
 maiusculo e prefixo onde estao localizadas:
 pdTRUE -> localizada em projdefs.h

 malloc e free no freertos:
 pvPortMalloc()
 pvPortFree()

 switched in/out swapped in/out -> suspend or resume task



 */


//necessario caso queira executar acao nas tarefas, como suspend, resume, delete
TaskHandle_t handlerPrimeiraTarefa;
TaskHandle_t handleSegundaTarefa;

QueueHandle_t handlerPrimeiraFila;
QueueHandle_t handlerSegundaFila;

typedef struct testeFilaStruct
{
    uint8_t id;
    char dados[20];
};

void primeiraTarefa(void *pvParameters)
{
    while(true)
    {
        vTaskDelay(1000 / portTICK_RATE_MS);
        printf("executando primeira tarefa no proc: %d\n", xPortGetCoreID());
    }
}

void segundaTarefa(void *pvParameters)
{
    while(true)
    {
        vTaskDelay(1000 / portTICK_RATE_MS);
        printf("executando segunda tarefa no proc: %d\n", xPortGetCoreID());
    }
}

void criandoTasks(void)
{
    //criando tarefa  no core APP
    xTaskCreatePinnedToCore(primeiraTarefa,
                            "nome para log 1",
                            2048,
                            NULL,
                            4,
                            &handlerPrimeiraTarefa,
                            APP_CPU_NUM);

    //criando tarefa no core PRO
    //se for somente criar a tarefa e nao for manipula la, nao precisa passar handler da tarefa
    //no sexto parametro
    xTaskCreatePinnedToCore(segundaTarefa,
                            "nome para log 2",
                            2048,
                            NULL,
                            8,
                            NULL,
                            PRO_CPU_NUM);
}

void manipulandoTasks(void)
{
    //parando uma tarefa, deve ser passado o handler da tarefa
    //vTaskDelay vai parar essa tarefa, as outras duas continuam rodando

//    vTaskDelay(5000 / portTICK_RATE_MS);
//    vTaskSuspend(handlerPrimeiraTarefa);
//    vTaskDelay(5000 / portTICK_RATE_MS);
//    vTaskResume(handlerPrimeiraTarefa);
//    vTaskDelay(5000 / portTICK_RATE_MS);
//    vTaskDelete(handlerPrimeiraTarefa);
}

void criandoFilas(void)
{
    handlerPrimeiraFila = xQueueCreate(10, sizeof(uint32_t));

    if (handlerPrimeiraFila == NULL)
    {
        printf("fila nao criada");
    }

    handlerSegundaFila = xQueueCreate(10, sizeof(struct testeFilaStruct *));

    if (handlerSegundaFila == NULL)
    {
        printf("fila nao criada");
    }
}

void escrevendoNasFilas(void)
{
    //checa se foi criada
    uint8_t dado = 1;
    if (handlerPrimeiraFila != NULL)
    {
        printf("escrevendo...\n");
        xQueueSend(handlerPrimeiraFila, (void *) &dado, (TickType_t) 0);
    }
}

void lendoFilas(void)
{
    uint8_t dadoRecebido = 0;
    if (handlerPrimeiraFila != NULL)
    {
        //mostra quantidade de dados que possui na fila
        printf("quantidade de dados na fila: %d", uxQueueMessagesWaiting(handlerPrimeiraFila));

        //executa o dequeue da fila (remove da fila)
        if (xQueueReceive(handlerPrimeiraFila, &dadoRecebido, 0))
        {
            printf("dado recebido: %d\n", dadoRecebido);
        }
        else
        {
            printf("nenhum dado recebido\n");
        }
    }
}

void app_main(void)
{
    criandoTasks();
//    manipulandoTasks();
//
//    criandoFilas();
//    lendoFilas();
//    escrevendoNasFilas();
}