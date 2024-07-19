
#ifndef DISTANCE_SENS_H
#define DISTANCE_SENS_H

#include <mutex>                /* Synchronization library                            */
#include <list>                 /* Utility library                                    */
#include "../../Util/Pins.h"    /* To determine the pins to be used                   */
#include <HCSR04.h>             /* Used to read from HCSR04 sensors                   */
#define FILTER_SAMPLE_NUM 10    /* Represents the order of the moving average filter, */
                                /* a higher number means a more stable output         */

/* The following class implements a threadable system to automatically use a generic amount of HCSR04 sensors
   that have the same trigger pin, and an unified trigger timer */
class DistanceSens{
    public:
        /* Class constructor */
        /* INPUT: The pin used to trigger the sensors, it has to be the same for all of them;
                  The amount of sensors being used;
                  The pins connected to the "ECHO" pin on the HCSR04;
                  A timer to pause execution that functions as a yield to the processor */
        DistanceSens(uint8_t trigPin, uint8_t sensorCount, uint8_t *sensorPins, uint16_t trigTimer);
        ~DistanceSens(); /* Class destructor */
      
        /* Function starts the connection to the sensors. Best utilized as a thread */
        void start(void); 

        /* Function to set a different timer to the one previously set
           INPUT: The number of milliseconds between each execution */
        void setTimer(uint16_t);

        /* Specialized functions */
        /* Gets first result, that is the sensor on the left side of the car     */
        double getDistanceL(void);  
        /* Gets second result, that is the sensor on the center side of the car  */
        double getDistanceC(void); 
        /* Gets third result, that is the sensor on the right side of the car    */
        double getDistanceR(void); 
    private:
         /* Amount of sensors being used at once                             */
        uint8_t _sensorCount; 
        /* Synchronization variable                                         */          
        std::mutex _mtx;                 
        /* Time between the executions of the "start" function              */
        uint16_t _trigTimer;           
        /* List holding the results of the readings,                        */
        /* where a reading is made up of the distances read by each sensor  */  
        std::list<double *> _distances;  
        /* Average between the previous FILTER_SAMPLE_NUM readings          */
        double *_averages;               
};

#endif