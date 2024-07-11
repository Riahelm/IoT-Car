
#ifndef DISTANCE_SENS_H
#define DISTANCE_SENS_H

#include "../../Util/Pins.h"
#include <mutex>
#include <list>
#include <HCSR04.h>
#define FILTER_SAMPLE_NUM 10 //Number of samples to be used in the moving average filter
class DistanceSens{
    public:
        //Trigger, then echo
        DistanceSens(int trigPin, int sensorCount, uint8_t *sensorPins, unsigned long trigTimer);
        ~DistanceSens();
      
        void start(void);

        void setTimer(unsigned long);
        double getDistanceL(void);
        double getDistanceC(void);
        double getDistanceR(void);
    private:
        int sensorCount;
        std::mutex mtx;
        unsigned long trigTimer;
        std::list<double *> distances;
        double *averages;
};

#endif