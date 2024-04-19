#ifndef STEPPER_H
#define STEPPER_H
#include "Arduino.h"
#include "Direction.h"

class Stepper{
  private:
    int pins[4];

  public:
    int speed;
    int stepsLeft;
    Direction dir;
    Stepper(int pin1, int pin2, int pin3, int pin4);

    void initPins();

    void oneStep(void);
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

};

#endif //STEPPER_H