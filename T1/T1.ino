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

#define N 5   /* Definição da quantia de filósofos */

/* Estados */
#define THINKING 0
#define HUNGRY 1
#define EATING 2

const char *s[3] = {"Pensando", "Com fome", "Comendo"};
const uint8_t f_id[N] = {0, 1, 2, 3, 4};  /* Sem isso os IDs passados por parâmetro na criação das tarefas dos filósofos pega valores errado (geralmente 0, 2, 3, 4, 5)*/
uint8_t states[N];                        /* Guarda o estado atual dos filósofos */

void State( unsigned portLONG id, unsigned portLONG state ){  /* Usando a função 'vPrintStringAndNumber' como base */
  /* Print the string, suspending the scheduler as method of mutual
  exclusion. */
  vTaskSuspendAll();
  {
      Serial.print("Filósofo ");
      Serial.print(id);
      Serial.print(" no estado: ");
      Serial.print(s[state]);
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

int left(int id){
    return((id+N-1)%N);
}

int right(int id){
  return((id+1)%N);
}

void philosopher(void *param){
  const uint8_t id = *(uint8_t *)param;
  vPrintStringAndNumber("Filósofo instanciado: ", id);
  for(;;){
    
  }
}

void setup() {
  Serial.begin(9600);
  
  uint8_t i;
  
  /* Instanciando filósofos */
  for(i = 0; i < N; i++){
    states[i] = 0;
    xTaskCreate(philosopher, NULL, 100, f_id + i, 1, NULL);
  }

  vTaskStartScheduler();
  for (;;);
}

void loop() {}
