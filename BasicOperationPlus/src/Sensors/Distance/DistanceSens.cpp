#include "DistanceSens.h"

DistanceSens::DistanceSens(const int(&leftPins)[2], const int(&centerPins)[2], const int(&rightPins)[2], unsigned long trigTimer)
{
    for (int i = 0; i < 3; i++) {distances = 0};
    
    this->trigTimer = trigTimer;
    dsSX = new HCSR04(leftPins[0], leftPins[1]);
    dsCX = new HCSR04(centerPins[0], centerPins[1]);
    dsDX = new HCSR04(rightPins[0], rightPins[1]);
};

DistanceSens::~DistanceSens()
{
};

void DistanceSens::start(void)
{
    while(true){
        distances[0] = dsSX.measureDistanceCm();
        distances[1] = dsCX.measureDistanceCm();
        distances[2] = dsDX.measureDistanceCm();
        delay(trigTimer);
    }
};

void DistanceSens::setTimer(int trigTimer)
{
    this->trigTimer = trigTimer;
}

float DistanceSens::getDistanceSX(void){
    return distances[0];
};
float DistanceSens::getDistanceCX(void){
    return distances[1];
};
float DistanceSens::getDistanceDX(void){
    return distances[2];
};
