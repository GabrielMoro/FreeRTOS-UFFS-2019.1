#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define CHAIR 2
#define CSTMR 2 /* Customer */

const TickType_t dlay = 1000 / portTICK_PERIOD_MS;    /* 1 segundo */

SemaphoreHandle_t semBarber, semCstmrs, mutex;

uint8_t CstmWaiting = 0;

void vPrintStringAndNumber( const char *pcString, unsigned portLONG ulValue ){
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

void barber(void *param){
  for(;;){
    xSemaphoreTake(semCstmrs, portMAX_DELAY);
    vPrintStringAndNumber("Barbeiro", 10);
  }
}

void customer(void *param){
  for(;;){
    vTaskDelay(random(500, 2000) / portTICK_PERIOD_MS);
    xSemaphoreTake(mutex, portMAX_DELAY);
    xSemaphoreGive(semCstmrs);
    vPrintStringAndNumber("Cliente", NULL);
    xSemaphoreGive(mutex);
  }
}

void setup() {
  Serial.begin(9600);

  semBarber = xSemaphoreCreateBinary();
  semCstmrs = xSemaphoreCreateCounting(CHAIR, 0);
  mutex = xSemaphoreCreateMutex();

  xTaskCreate(barber, NULL, 75, NULL, 1, NULL);

  for(uint8_t i = 0; i < CSTMR; i++)
    xTaskCreate(customer, NULL, 75, NULL, 1, NULL);
  /* ... */

  vTaskStartScheduler();
  for (;;);
}

void loop() {}
