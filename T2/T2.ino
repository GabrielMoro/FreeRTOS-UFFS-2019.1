#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define CHAIR 2
#define CSTMR 2 /* Customer */

const TickType_t dlay = 1000 / portTICK_PERIOD_MS;    /* 1 segundo */

SemaphoreHandle_t semBarber, semCstmrs;

uint8_t freeChairs = CHAIR;

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
     
  }
}

void customer(void *param){
  for(;;){
    
  }
}

void setup() {
  Serial.begin(9600);

  semBarber = xSemaphoreCreateBinary();
  semCstmrs = xSemaphoreCreateCounting(CHAIR, 0);
  /* ... */

  vTaskStartScheduler();
  for (;;);
}

void loop() {}
