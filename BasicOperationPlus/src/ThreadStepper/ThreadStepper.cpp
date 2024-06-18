#include "ThreadStepper.h"
#include "Arduino.h"

ThreadStepper::ThreadStepper(int pin1, int pin2, int pin3, int pin4) 
{
    pins[0] = pin1;
    pins[1] = pin2;
    pins[2] = pin3;
    pins[3] = pin4;
    ThreadStepper::speed = 2;
    ThreadStepper::dir   = Direction::FORWARD;
}

void ThreadStepper::initPins(void)
{
    for(int i = 0; i < 4; i++){
        pinMode(pins[i], OUTPUT);
    }
}

void ThreadStepper::oneStep(void)
{
    step(1);
}

void ThreadStepper::oneStep(Direction dir)
{
    step(dir, 1);
}

void ThreadStepper::step(Direction dir, int stepNum)
{
    setDirection(dir);
    step(stepNum);
}

void ThreadStepper::stepAngle(int angle)
{
    step(dir, angle/5.625);
}

void ThreadStepper::stepAngle(Direction dir, int angle)
{
    setDirection(dir);
    stepAngle(angle);
}

void ThreadStepper::step(int stepNum)
{
    ThreadStepper::stepsLeft = stepNum;
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



void ThreadStepper::setDirection(Direction dir)
{
    ThreadStepper::dir = dir;
}

Direction ThreadStepper::getDirection()
{
    return dir;
}

void ThreadStepper::setSpeed(int speed)
{
    ThreadStepper::speed = speed;
}

int ThreadStepper::getSpeed()
{
    return ThreadStepper::speed;
}

int ThreadStepper::getRemainingSteps()
{
    return ThreadStepper::stepsLeft;
}