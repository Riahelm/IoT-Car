#include "src/Util/Pins.h"
#include "src/Steppers/Steppers.h"
#include <thread>
#include <list>

int leftMotorPins[4]  = {MOTORSTEP_SX_1, MOTORSTEP_SX_2, MOTORSTEP_SX_3, MOTORSTEP_SX_4};
int rightMotorPins[4] = {MOTORSTEP_DX_1, MOTORSTEP_DX_2, MOTORSTEP_DX_3, MOTORSTEP_DX_4}; 

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

void firstMotorCode(void *pvParam){
  BYJStepper step(15,2,0,4);
  for(;;){
    Serial.println("Ciao1");
  }
}

void secondMotorCode(void *pvParam){
  for(;;){
    Serial.println("Ciao2");
  }
}


void loop() {
  Steppers motors = Steppers(leftMotorPins, rightMotorPins);
  std::list<motorInstructions
  std::thread motorThread(&Steppers::run, &motors, )
  
}