#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define SEATS 2
#define CSTMR 5 /* Customer */

const TickType_t dlay = 1000 / portTICK_PERIOD_MS;    /* 1 segundo */

TaskHandle_t barberHandle;
SemaphoreHandle_t seatsMutex;

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
    vPrintString("(Barber)");

    vTaskDelay(dlay);
    vPrintString("\n(Barber) Terminou...");
  }
}

void customer(void *param) {
  for (;;) {
    vTaskDelay(random(1500, 3000) / portTICK_PERIOD_MS);
    vPrintString("\n(Customer) Entrando...");
  }
}

void setup() {
  Serial.begin(9600);

  seatsMutex = xSemaphoreCreateMutex();
  xTaskCreate(barber, NULL, 80, NULL, 1, NULL);
  for (uint8_t i = 0; i < CSTMR; i++)
    xTaskCreate(customer, NULL, 100, NULL, 1, NULL);
  vTaskStartScheduler();
  for (;;);
}

void loop() {}
