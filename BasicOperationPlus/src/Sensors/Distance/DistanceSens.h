
#ifndef DISTANCE_SENS_H
#define DISTANCE_SENS_H

#include "../../Util/Pins.h"

#include <HCSR04.h>

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
        unsigned long trigTimer;
        double *distances;
};

#endif