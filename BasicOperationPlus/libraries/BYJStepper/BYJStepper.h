#ifndef _STEPPER_H_
#define _STEPPER_H_
#include <Arduino.h>
#include "../Direction/Direction.h"
class BYJStepper{
public:
  
  BYJStepper(int pin1, int pin2, int pin3, int pin4);

  void initPins();

  void oneStep();
  void oneStep(Direction dir);
  void step(int stepNum);
  void step(Direction dir, int stepNum);
  void stepAngle(int angle);
  void stepAngle(Direction dir, int angle);

  void setDirection(Direction dir);
  Direction getDirection();

  void setSpeed(int speed);
  int getSpeed();

  int getRemainingSteps();

  int speed;
  int stepsLeft;
  Direction dir;
  
private:
  int pins[4];
};

#endif //_STEPPER_H_