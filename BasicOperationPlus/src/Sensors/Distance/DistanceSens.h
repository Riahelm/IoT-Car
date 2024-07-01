
#ifndef DISTANCE_SENS_H
#define DISTANCE_SENS_H

#include "../../Util/Pins.h"

#include <HCSR04.h>

class DistanceSens{
    public:
        //Trigger, then echo
        DistanceSens(const int (&leftPins)[2], const int (&centerPins)[2], const int (&rightPins)[2], unsigned long trigTimer);
        ~DistanceSens();
        void start(void);

        void setTimer(int);
        float getDistanceSX(void);
        float getDistanceCX(void);
        float getDistanceDX(void);
    private:
        int trigTimer;
        float distances[3];
        HCSR04 *dsSX;
        HCSR04 *dsCX;
        HCSR04 *dsDX;
};

#endif