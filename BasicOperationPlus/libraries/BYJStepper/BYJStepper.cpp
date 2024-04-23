#include "BYJStepper.h"
#include "Arduino.h"

BYJStepper::BYJStepper(int pin1, int pin2, int pin3, int pin4)
{
    pins[0] = pin1;
    pins[1] = pin2;
    pins[2] = pin3;
    pins[3] = pin4;
    BYJStepper::speed= 2;
    BYJStepper::dir  = Direction::FORWARD;
}

void BYJStepper::initPins()
{
    for(int i = 0; i < 4; i++){
        pinMode(pins[i], OUTPUT);
    }
}

void BYJStepper::oneStep(void)
{
    step(1);
}

void BYJStepper::oneStep(Direction dir)
{
    step(dir);
}

void BYJStepper::step(Direction dir, int stepNum)
{
    setDirection(dir);
    step(stepNum);
}

void BYJStepper::stepAngle(int angle)
{
    step(dir, angle/5.625);
}

void BYJStepper::stepAngle(Direction dir, int angle)
{
    setDirection(dir);
    stepAngle(angle);
}

void BYJStepper::step(int stepNum)
{
    BYJStepper::stepsLeft = stepNum;
    int count;
    if(dir == Direction::FORWARD){
        for (int i = 0; i < stepNum; i++)
        {
            count = i % 4;
            digitalWrite(pins[count], HIGH);
            delay(speed);
            digitalWrite(pins[count], LOW);
        };
    }else
    {
        for (int i = stepNum; i > 0; i--)
        {
            count = i % 4;
            digitalWrite(pins[count], HIGH);
            delay(speed);
            digitalWrite(pins[count], LOW);
        };
    };
}



void BYJStepper::setDirection(Direction dir)
{
    BYJStepper::dir = dir;
}

Direction BYJStepper::getDirection()
{
    return dir;
}

void BYJStepper::setSpeed(int speed)
{
    BYJStepper::speed = speed;
}

int BYJStepper::getSpeed()
{
    return BYJStepper::speed;
}

int BYJStepper::getRemainingSteps()
{
    return BYJStepper::stepsLeft;
}
