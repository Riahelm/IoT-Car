/*
  Basic OPeration Stepper Motor that uses ULN2003
  Author: Bonezegei (Jofel Batutay)
  Date:  March 2024
*/

#include "Bonezegei_ULN2003_Stepper.h"
//Param1 Input Pin1
//Param1 Input Pin2
//Param1 Input Pin3
//Param1 Input Pin4
Bonezegei_ULN2003_Stepper Stepper(16, 17, 18, 19);

// Set This According to your Preference
#define FORWARD 1
#define REVERSE 0

void setup() {
  //Inititalize Pins
  Stepper.begin();

  // Speed in Milli seconds per step
  // Default Value is 3 
  Stepper.setSpeed(2);
}

void loop() {
  Stepper.step(FORWARD, 2048);
  delay(500);
}
