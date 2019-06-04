#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define SEATS 2
#define CSTMR 5 /* Customer */

const TickType_t dlay = 1000 / portTICK_PERIOD_MS;    /* 1 segundo */

void setup() {
  

  vTaskStartScheduler();
  for (;;);
}

void loop() {}
