/*
  FreeRTOS.org V5.0.4 - Copyright (C) 2003-2008 Richard Barry.

  This file is part of the FreeRTOS.org distribution.

  FreeRTOS.org is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  FreeRTOS.org is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with FreeRTOS.org; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  A special exception to the GPL can be applied should you wish to distribute
  a combined work that includes FreeRTOS.org, without being obliged to provide
  the source code for any proprietary components.  See the licensing section
  of http://www.FreeRTOS.org for full details of how and when the exception
  can be applied.

    ***************************************************************************
    ***************************************************************************
    *                                                                         *
    * SAVE TIME AND MONEY!  We can port FreeRTOS.org to your own hardware,    *
    * and even write all or part of your application on your behalf.          *
    * See http://www.OpenRTOS.com for details of the services we provide to   *
    * expedite your project.                                                  *
    *                                                                         *
    ***************************************************************************
    ***************************************************************************

  Please ensure to read the configuration and relevant port sections of the
  online documentation.

  http://www.FreeRTOS.org - Documentation, latest information, license and
  contact details.

  http://www.SafeRTOS.com - A version that is certified for use in safety
  critical systems.

  http://www.OpenRTOS.com - Commercial support, development, porting,
  licensing and training services.
*/

/* FreeRTOS.org includes. */
#include <Arduino_FreeRTOS.h>
#include <queue.h>

#define N 5   /* Definição da quantia de filósofos */

/* Estados */
#define THINKING 0
#define HUNGRY 1
#define EATING 2

/* Constantes */
const char *s[3] = {"Pensando", "Com fome", "Comendo"};
const uint8_t f_id[N] = {0, 1, 2, 3, 4};              /* Sem isso os IDs passados por parâmetro na criação das tarefas dos filósofos pega valores errado (geralmente 0, 2, 3, 4, 5)*/
const TickType_t dlay = 1000 / portTICK_PERIOD_MS;  /* 1 segundo */

uint8_t states[N], bfr = 1;      /* Guarda o estado atual dos filósofos, buffer */

QueueHandle_t forks[N], take; /* Estado dos garfos*/

void State( unsigned portLONG id, unsigned portLONG state ){  /* Usando a função 'vPrintStringAndNumber' como base */
  /* Print the string, suspending the scheduler as method of mutual
  exclusion. */
  vTaskSuspendAll();
  {
      Serial.print("Filósofo ");
      Serial.print(id);
      Serial.print(" no estado: ");
      Serial.print(s[state]);
      Serial.print("\n");
      Serial.flush();
  }
  xTaskResumeAll();

  /* Allow any key to stop the application running. */
  if( Serial.available() )
  {
    vTaskEndScheduler();
  }
}

void vPrintStringAndNumber( const char *pcString, unsigned portLONG ulValue )
{
  /* Print the string, suspending the scheduler as method of mutual
  exclusion. */
  vTaskSuspendAll();
  {
      Serial.print(pcString);
      Serial.write(' ');
      Serial.println(ulValue);
      Serial.flush();
  }
  xTaskResumeAll();

  /* Allow any key to stop the application running. */
  if( Serial.available() )
  {
    vTaskEndScheduler();
  }
}

int left(uint8_t id){
    return((id+N-1)%N);
}

int right(uint8_t id){
  return((id+1)%N);
}

void takeFork(void *param){
  uint8_t phi;  /* Usado para pegar o id do filósofo que tentar comer */
  
  for(;;){
    xQueueReceive(take, &phi, portMAX_DELAY); /* 'phi' contém o id do filósofo que tentar comer, caso a fila esteja vazia a tarefa bloqueia */
    if(states[phi] == HUNGRY && states[left(phi)] != EATING && states[right(phi)] != EATING) {  /* Caso os filósofos adjacentes estejam comendo os garfos estão ocupados */
      states[phi] = EATING;                   /* Caso os garfos estejam disponíveis e o filósofo com fome ele pega os garfos e come */
      xQueueSend(forks[phi], &bfr, portMAX_DELAY);
      State(phi, states[phi]);
    }
  }
}

void philosopher(void *param){
  const uint8_t id = *(uint8_t *)param;
  
  vPrintStringAndNumber("Filósofo instanciado: ", id);
  State(id, states[id]);

  for(;;){
    vTaskDelay(dlay);
    
    states[id] = HUNGRY;
    State(id, states[id]);
    
    xQueueSend(take, &id, portMAX_DELAY);               /* Tenta pegar os garfos */
    xQueueReceive(forks[id], &bfr, portMAX_DELAY);

    states[id] = THINKING;
    State(id, states[id]);

    vTaskDelay(dlay);

    xQueueSend(take, &f_id[left(id)], portMAX_DELAY);   /* Testando garfo da esquerda */
    xQueueSend(take, &f_id[right(id)], portMAX_DELAY);  /* Testando garfo da direita */  
  }
}

void setup() {
  Serial.begin(9600);
  
  uint8_t i;
  take = xQueueCreate(1, sizeof(uint8_t));              /* Fila de tamanho 1 na qual os filósofos enviam o seu id para ser utilizado na função 'takeFork' */
  
  /* Instanciando filósofos */
  for(i = 0; i < N; i++){
    states[i] = 0;
    xTaskCreate(philosopher, NULL, 75, (f_id + i), 1, NULL);
  }

  /* Inicializa filas de tamanho 1 para os garfos */
  for(i = 0; i < N; i++){
    forks[i] = xQueueCreate(1, sizeof(uint8_t));
  }

  xTaskCreate(takeFork, NULL, 100, NULL, 2, NULL);

  vTaskStartScheduler();
  for (;;);
}

void loop() {}
