#include "DistanceSens.h"

DistanceSens::DistanceSens(int trigPin, int sensorCount, uint8_t *sensorPins, unsigned long trigTimer)
{
    this->trigTimer = trigTimer;
    distances = new double[sensorCount];
    HCSR04.begin(trigPin, sensorPins, sensorCount);
}

DistanceSens::~DistanceSens() {};

void DistanceSens::start(void)
{
    while(true){
        distances = HCSR04.measureDistanceMm();
        delay(trigTimer);
    }
};

void DistanceSens::setTimer(unsigned long trigTimer)
{
    this->trigTimer = trigTimer;
}

double DistanceSens::getDistanceL(void){
    return distances[0];
};
double DistanceSens::getDistanceC(void){
    return distances[1];
};
double DistanceSens::getDistanceR(void){
    return distances[2];
};
