#include "src/Util/Pins.h"
#include "src/Steppers/Steppers.h"

int leftMotorPins[4]  = {MOTORSTEP_SX_1, MOTORSTEP_SX_2, MOTORSTEP_SX_3, MOTORSTEP_SX_4};
int rightMotorPins[4] = {MOTORSTEP_DX_1, MOTORSTEP_DX_2, MOTORSTEP_DX_3, MOTORSTEP_DX_4}; 

void setup() {

  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

}

void loop() {
  Steppers motors = Steppers(leftMotorPins, rightMotorPins);

  motors.goForwards(500);
  motors.turnLeft(G45);
  motors.turnLeft(G45);
  motors.turnRight(G90 + G15);
  
}