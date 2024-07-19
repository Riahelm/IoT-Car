#include "Arduino.h"
#include <thread>
#include "src/Util/Pins.h"
#include "src/Actuators/Steppers/Steppers.h"
#include "src/Sensors/Distance/DistanceSens.h"
#include "src/Communication/MyWiFi/MyWiFi.h"
#include "src/Communication/MyTelnet/MyTelnet.h"

/* Store all pins in the respective structures */
uint8_t leftMotorPins[4]    = {MOTORSTEP_L_1, MOTORSTEP_L_2, MOTORSTEP_L_3, MOTORSTEP_L_4};
uint8_t rightMotorPins[4]   = {MOTORSTEP_R_1, MOTORSTEP_R_2, MOTORSTEP_R_3, MOTORSTEP_R_4}; 
uint8_t sensorPins[3]       = {DIST_L_ECHO, DIST_C_ECHO, DIST_R_ECHO};
uint8_t triggerPin          = DIST_TRIG;

Steppers *motors;
DistanceSens *sensors;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  setupSteppers();
  setupSensors();
  setupWiFi();
  setupTelnet();
  delay(2500);                                                /* Wait for the sensors to stabilize */
}

void setupTelnet(){
  /* Set steppers for telnet */
  setSteppers(motors);
  /* Set sensors for telnet */
  setSensors(sensors);
  /* If device isn't connected, attempt connection through default settings */
  if(!isConnected){
    setupWiFi();
  }
  /* Initialize telnet */
  initTelnet();
  /* Create thread for telnet.loop */
  std::thread telnetT(startTelnet);
  /* Allows for thread to run independently from setup() */
  telnetT.detach();
}

void setupSteppers(){
  motors  = new Steppers(leftMotorPins, rightMotorPins);      /* Create stepper object */
  std::thread motorT(&Steppers::start, motors);               /* Create stepper thread */
  motorT.detach();                                            /* Allows for thread to run independently from setup() */
}

void setupSensors(){
  sensors = new DistanceSens(triggerPin, 3, sensorPins, 20);  /* Create sensor  object */
  std::thread sensorT(&DistanceSens::start, sensors);         /* Create sensor  thread */
  sensorT.detach();                                           /* Allows for thread to run independently from setup() */
}

void loop() {
}
