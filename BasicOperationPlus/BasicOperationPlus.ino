#include "Arduino.h"
#include "src/Util/Pins.h"
#include "src/Steppers/Steppers.h"
#include "src/Sensors/Distance/DistanceSens.h"
#include <HCSR04.h>
#include <thread>
#include <list>

int leftMotorPins[4]    = {MOTORSTEP_L_1, MOTORSTEP_L_2, MOTORSTEP_L_3, MOTORSTEP_L_4};
int rightMotorPins[4]   = {MOTORSTEP_R_1, MOTORSTEP_R_2, MOTORSTEP_R_3, MOTORSTEP_R_4}; 
int triggerPin          = DIST_TRIG;
uint8_t sensorPins[3] = {DIST_L_ECHO, DIST_C_ECHO, DIST_R_ECHO};

Steppers motors(leftMotorPins, rightMotorPins);
DistanceSens sensors(triggerPin, 3, sensorPins, 20);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  //std::thread motorT(&Steppers::start, &motors);
  std::thread sensorT(&DistanceSens::start, &sensors);
  //delay(250); 
  //motors.addInstruction(Steppers::GoForwards, 500);
  while(true){
    Serial.println("Left: " + String(sensors.getDistanceL()));
    Serial.println("Central: " + String(sensors.getDistanceC()));
    Serial.println("Right: " + String(sensors.getDistanceR()));
    delay(500);
  }
  while(true);
}