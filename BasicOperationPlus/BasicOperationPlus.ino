/*
  Basic OPeration Stepper Motor that uses ULN2003
  Author: Bonezegei (Jofel Batutay)
  Date:  March 2024
*/

#include "Bonezegei_ULN2003_Stepper.h"
//Bonezegei_ULN2003_Stepper Stepper(15, 2, 0, 4);
//Bonezegei_ULN2003_Stepper Stepper(16, 17, 5, 18);




// Set This According to your Preference
#define FORWARD 1
#define REVERSE 0

void setup() {
  //Inititalize Pins
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
                  "Motor1",
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

void loop() {

}

void firstMotorCode(void *pvParameters){
  Bonezegei_ULN2003_Stepper myStepper(15,2,0,4);
  myStepper.begin();
  for(int i = 0;;i++){ 
    myStepper.setSpeed(i);
    myStepper.step(1, 100);
  }
}

void secondMotorCode(void *pvParameters){
  Bonezegei_ULN2003_Stepper myStepper(16, 17, 5, 18);
  myStepper.begin();
  for(int i = 0;;i++){
    myStepper.setSpeed(i);
    myStepper.step(1, 100);
  }
}