#include "src/Util/Pins.h"
#include "src/Steppers/Steppers.h"
#include <thread>
#include <list>

int leftMotorPins[4]  = {MOTORSTEP_SX_1, MOTORSTEP_SX_2, MOTORSTEP_SX_3, MOTORSTEP_SX_4};
int rightMotorPins[4] = {MOTORSTEP_DX_1, MOTORSTEP_DX_2, MOTORSTEP_DX_3, MOTORSTEP_DX_4}; 

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  delay(250);
}

void loop() {
  Steppers motors(leftMotorPins, rightMotorPins);
  std::thread obj(&Steppers::start, &motors);
  motors.addInstruction(Steppers::GoForwards, 500);
  motors.addInstruction(Steppers::TurnLeft, G45);
  motors.addInstruction(Steppers::GoBackwards, 500);
  motors.addInstruction(Steppers::TurnRight, G45);
  delay(250);
  for (int i = 0; i < 5; i++)
  {
    motors.addInstruction(Steppers::GoForwards, 500);
    Serial.println("Communicating as it works!");
    delay(500);
  }
  while(true);
}