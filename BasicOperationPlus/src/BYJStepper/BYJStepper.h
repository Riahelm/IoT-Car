#ifndef _STEPPER_H_
#define _STEPPER_H_
#include <Arduino.h>
#include "../Direction/Direction.h"
class BYJStepper{
public:
  
  BYJStepper(int, int, int, int);

  void initPins(void);

  void oneStep(void);
  void oneStep(Direction);
  void step(int);
  void step(Direction, int);
  void stepAngle(int);
  void stepAngle(Direction, int);

  void setDirection(Direction);
  Direction getDirection();

  void setSpeed(int);
  int getSpeed();

  int getRemainingSteps();

  int speed;
  int stepsLeft;
  Direction dir;
  
private:
  int pins[4];
};

#endif //_STEPPER_H_