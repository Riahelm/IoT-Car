#include "DistanceSens.h"
DistanceSens::DistanceSens(int trigPin, int sensorCount, uint8_t *sensorPins, unsigned long trigTimer)
{
    this->trigTimer = trigTimer;
    this->sensorCount = sensorCount;
    averages = (double *) calloc(sensorCount, sizeof(double));
    for (int i = 0; i < sensorCount; i++){averages[i] = 0;};
    distances.assign(FILTER_SAMPLE_NUM, averages);
    HCSR04.begin(trigPin, sensorPins, sensorCount);
}

DistanceSens::~DistanceSens() {}

void DistanceSens::start(void)
{
    while(true){
        double *newDistances = HCSR04.measureDistanceMm();
        distances.push_front(newDistances);
        {
            std::unique_lock<std::mutex> lock(mtx);
            for (int i = 0; i < sensorCount; i++)
            {
                averages[i] += newDistances[i];
                Serial.println("TMP: " + String(averages[i]));
                averages[i] -= distances.back()[i];
            }
        }

        if(distances.size() > FILTER_SAMPLE_NUM){
            distances.pop_back();
        }
        delay(trigTimer);
    }
};

void DistanceSens::setTimer(unsigned long trigTimer)
{
    this->trigTimer = trigTimer;
}

double DistanceSens::getDistanceL(void){
    std::unique_lock<std::mutex> lock(mtx);
    return averages[0] / FILTER_SAMPLE_NUM;
};
double DistanceSens::getDistanceC(void){
    std::unique_lock<std::mutex> lock(mtx);
    return averages[1]/ FILTER_SAMPLE_NUM;
};
double DistanceSens::getDistanceR(void){
    std::unique_lock<std::mutex> lock(mtx);
    return averages[2]/ FILTER_SAMPLE_NUM;
};
