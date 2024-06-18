#include "src\ThreadStepper\ThreadStepper.h"

void setup() {
  BaseType_t returnedVal;
  TaskHandle_t xHandle1 = NULL;
  TaskHandle_t xHandle2 = NULL;
  Serial.begin(115200);
  returnedVal = xTaskCreate(
                  firstMotorCode,
                  "Motor1",
                  1000,
                  (void *) 1,
                  tskIDLE_PRIORITY,
                  &xHandle1);

  if(returnedVal != pdPASS){
    Serial.println("FAILURE 1");
  }

  returnedVal = xTaskCreate(
                  secondMotorCode,
                  "Motor2",
                  1000,
                  (void *) 1,
                  tskIDLE_PRIORITY,
                  &xHandle2);

  if(returnedVal != pdPASS){
    Serial.println("FAILURE 2");
  }

  delay(50000);
  vTaskDelete(xHandle2);
}

void firstMotorCode(void *pvParam){
  //(ThreadStepper) pvParam;
  for(;;){
    
  }
}

void secondMotorCode(void *pvParam){
  for(;;){
    Serial.println("Ciao2");
  }
}


void loop() {
}