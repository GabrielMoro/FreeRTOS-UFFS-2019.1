#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define SEATS 2
#define CSTMR 5 /* Customer */

const TickType_t dlay = 1000 / portTICK_PERIOD_MS;    /* 1 segundo */

TaskHandle_t *barberHandle;

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
    vPrintString("Barber");
  }
}

void customer(void *param){
  for(;;){
    vPrintString("Customer");
  }
}

void setup() {
  Serial.begin(9600);

  xTaskCreate(barber, NULL, 70, NULL, 1, barberHandle);

  for(uint8_t i = 0; i < CSTMR; i++)
    xTaskCreate(customer, NULL, 70, NULL, 1, NULL);

  vTaskStartScheduler();
  for (;;);
}

void loop() {}
