#include "Stepper.h"
#include "Arduino.h"
using namespace Stepper;
Stepper::Stepper(int pin1, int pin2, int pin3, int pin4)
{
    pins[0] = pin1;
    pins[1] = pin2;
    pins[2] = pin3;
    pins[4] = pin4;
    Stepper::speed= 2;
    Stepper::dir  = Direction::FORWARD;
}

void Stepper::initPins()
{
    for(int i = 0; i < 4; i++){
        pinMode(pins[i], OUTPUT);
    }
}

void Stepper::oneStep(void)
{
    step(1);
}

void Stepper::oneStep(Direction dir)
{
    step(dir);
}

void Stepper::step(Direction dir, int stepNum)
{
    setDirection(dir);
    step(stepNum);
}

void Stepper::stepAngle(int angle)
{
    step(dir, angle/5.625);
}

void Stepper::stepAngle(Direction dir, int angle)
{
    setDirection(dir);
    stepAngle(angle);
}

void Stepper::step(int stepNum)
{
    Stepper::stepsLeft = stepNum;
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



void Stepper::setDirection(Direction dir)
{
    Stepper::dir = dir;
}

Direction Stepper::getDirection()
{
    return dir;
}

void Stepper::setSpeed(int speed)
{
    Stepper::speed = speed;
}

int Stepper::getSpeed()
{
    return Stepper::speed;
}

int Stepper::getRemainingSteps()
{
    return Stepper::stepsLeft;
}
