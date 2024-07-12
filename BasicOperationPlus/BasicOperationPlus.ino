#include "Arduino.h"
#include "src/Util/Pins.h"
#include "src/Steppers/Steppers.h"
#include "src/Sensors/Distance/DistanceSens.h"
#include "src/WiFi/WiFi.h"
#include <thread>

/* Store all pins in the respective structures */
uint8_t leftMotorPins[4]    = {MOTORSTEP_L_1, MOTORSTEP_L_2, MOTORSTEP_L_3, MOTORSTEP_L_4};
uint8_t rightMotorPins[4]   = {MOTORSTEP_R_1, MOTORSTEP_R_2, MOTORSTEP_R_3, MOTORSTEP_R_4}; 
uint8_t triggerPin          = DIST_TRIG;
uint8_t sensorPins[3] = {DIST_L_ECHO, DIST_C_ECHO, DIST_R_ECHO};

Steppers *motors;
DistanceSens *sensors;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  motors  = new Steppers(leftMotorPins, rightMotorPins);      /* Create stepper object */
  sensors = new DistanceSens(triggerPin, 3, sensorPins, 20);  /* Create sensor  object */
  std::thread motorT(&Steppers::start, motors);               /* Create stepper thread */
  std::thread sensorT(&DistanceSens::start, sensors);         /* Create sensor  thread */
  motorT.detach();                                            /* Allows for thread to run independently from setup() */
  sensorT.detach();                                           /* Allows for thread to run independently from setup() */
  delay(2500);                                                /* Wait for the sensors to stabilize */

  
}


void loop() {

  motors->goForwards(100);
  motors->turnLeft(360);
  motors->goForwards(100);
  motors->turnRight(360);
  motors->goBackwards(200);

  while(true);
}
