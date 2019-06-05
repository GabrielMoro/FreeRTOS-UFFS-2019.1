#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define SEATS 2
#define CSTMR 5 /* Customer */

const TickType_t dlay = 1000 / portTICK_PERIOD_MS;    /* 1 segundo */

TaskHandle_t barberHandle;
SemaphoreHandle_t seatsMutex;

uint8_t freeSeats = SEATS;

void vPrintString( const char *pcString ) {
  vTaskSuspendAll();
  {
    Serial.print(pcString);
    Serial.println("");
    Serial.flush();
  }
  xTaskResumeAll();

  if ( Serial.available() )
  {
    vTaskEndScheduler();
  }
}

void barber(void *param) {
  for (;;) {
    if (freeSeats == SEATS)
      vPrintString("(Barber) Dormindo...");
    ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
    xSemaphoreTake(seatsMutex, portMAX_DELAY);
    freeSeats += 1;
    vPrintString("\n(Barber) Trabalhando...");
    Serial.print("(Barber) Bancos livres: ");
    Serial.println(freeSeats);
    xSemaphoreGive(seatsMutex);
    vTaskDelay(dlay);
    vPrintString("\n(Barber) Terminou...");
  }
}

void customer(void *param) {
  for (;;) {
    vTaskDelay(random(1500, 3000) / portTICK_PERIOD_MS);
    xSemaphoreTake(seatsMutex, portMAX_DELAY);
    vPrintString("\n(Customer) Entrando...");
    if (freeSeats > 0) {
      freeSeats -= 1;
      vPrintString("(Customer) Esperando...");
      Serial.print("(Customer) Bancos livres: ");
      Serial.println(freeSeats);
      xTaskNotifyGive(barberHandle);
      xSemaphoreGive(seatsMutex);
    } else {
      vPrintString("(Customer) Barbearia cheia!!");
      xSemaphoreGive(seatsMutex);
    }
  }
}

void setup() {
  Serial.begin(9600);

  seatsMutex = xSemaphoreCreateMutex();
  xTaskCreate(barber, NULL, 80, NULL, 1, &barberHandle);
  for (uint8_t i = 0; i < CSTMR; i++)
    xTaskCreate(customer, NULL, 100, NULL, 1, NULL);
  vTaskStartScheduler();
  for (;;);
}

void loop() {}
