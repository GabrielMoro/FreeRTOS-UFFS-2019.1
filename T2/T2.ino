#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define CHAIR 1
#define CSTMR 1 /* Customer */

const TickType_t dlay = 1000 / portTICK_PERIOD_MS;    /* 1 segundo */

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

}

void customer(void *param){
  
}

void setup() {
  Serial.begin(9600);

  /* ... */

  vTaskStartScheduler();
  for (;;);
}

void loop() {}
