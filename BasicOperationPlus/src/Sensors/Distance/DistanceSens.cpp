#include "DistanceSens.h"
DistanceSens::DistanceSens(int trigPin, int sensorCount, uint8_t *sensorPins, unsigned long trigTimer)
{
    this->trigTimer = trigTimer;
    this->sensorCount = sensorCount;
    averages = (double *) calloc(sensorCount, sizeof(double));
    for (int i = 0; i < sensorCount; i++){averages[i] = 0;};
    for (int i = 0; i < FILTER_SAMPLE_NUM; i++){
        distances.push_front((double *) calloc(sensorCount, sizeof(double)));
    };
    HCSR04.begin(trigPin, sensorPins, sensorCount);
}

DistanceSens::~DistanceSens() {}

void DistanceSens::start(void)
{
    while(true){

        double *measures = (double *) calloc(sensorCount, sizeof(double));
        double *tmp = HCSR04.measureDistanceMm();

        for(int i = 0; i < sensorCount; i++){
            measures[i] = tmp[i];
        }
        distances.push_front(measures);
        {
            std::unique_lock<std::mutex> lock(mtx);
            for (int i = 0; i < sensorCount; i++)
            {   
                averages[i] += distances.front()[i] / FILTER_SAMPLE_NUM;
                averages[i] -= distances.back()[i] / FILTER_SAMPLE_NUM;
            }
        }
        distances.pop_back();
        delay(trigTimer);

    }
};

void DistanceSens::setTimer(unsigned long trigTimer)
{
    this->trigTimer = trigTimer;
}

double DistanceSens::getDistanceL(void){
    std::unique_lock<std::mutex> lock(mtx);
    return averages[0];
};
double DistanceSens::getDistanceC(void){
    std::unique_lock<std::mutex> lock(mtx);
    return averages[1];
};
double DistanceSens::getDistanceR(void){
    std::unique_lock<std::mutex> lock(mtx);
    return averages[2];
};
