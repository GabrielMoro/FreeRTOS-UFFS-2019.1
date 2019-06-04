#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define SEATS 3
#define CSTMR 5 /* Customer */

const TickType_t dlay = 1000 / portTICK_PERIOD_MS;    /* 1 segundo */

SemaphoreHandle_t semBarber, semCstmrs, mutex;

uint8_t freeSeats = SEATS;

void vPrintString( const char *pcString ){
  vTaskSuspendAll();
  {
      Serial.print(pcString);
      Serial.println("");
      Serial.flush();
  }
  xTaskResumeAll();

  if( Serial.available() )
  {
    vTaskEndScheduler();
  }
}

void barber(void *param){
  for(;;){
    
    xSemaphoreTake(semCstmrs, portMAX_DELAY); // Esperando clientes
    xSemaphoreTake(mutex, portMAX_DELAY);
    freeSeats += 1;
    vPrintString("Barbeiro trabalhando...");
    xSemaphoreGive(semBarber);
    xSemaphoreGive(mutex);
    vTaskDelay(dlay);
  }
}

void customer(void *param){
  for(;;){
    vTaskDelay(random(500, 1500) / portTICK_PERIOD_MS);
    xSemaphoreTake(mutex, portMAX_DELAY);
    if(freeSeats > 0){
      freeSeats -= 1;
      vPrintString("Esperando...");
      xSemaphoreGive(semCstmrs);
      xSemaphoreGive(mutex);
      xSemaphoreTake(semBarber, portMAX_DELAY);
      vPrintString("Cortando o cabelo...");
    }else{
      vPrintString("Barbearia cheia");
      xSemaphoreGive(mutex);
    }
  }
}

void setup() {
  Serial.begin(9600);

  semBarber = xSemaphoreCreateBinary();
  semCstmrs = xSemaphoreCreateCounting(SEATS, 0);
  mutex = xSemaphoreCreateMutex();

  xTaskCreate(barber, NULL, 70, NULL, 1, NULL);

  for(uint8_t i = 0; i < CSTMR; i++)
    xTaskCreate(customer, NULL, 70, NULL, 1, NULL);
  /* ... */

  vTaskStartScheduler();
  for (;;);
}

void loop() {}
