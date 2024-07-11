#include "DistanceSens.h"

/* Class constructor */
DistanceSens::DistanceSens(uint8_t trigPin, uint8_t sensorCount, uint8_t *sensorPins, uint16_t trigTimer)
{
    this->trigTimer = trigTimer;
    this->sensorCount = sensorCount;
    averages = (double *) calloc(sensorCount, sizeof(double));  /* Averages is an array holding each result */ 
    for (int i = 0; i < sensorCount; i++){averages[i] = 0;};    /* Initialize the average to 0 */
    for (int i = 0; i < FILTER_SAMPLE_NUM; i++){                /* Assign a memory location to each distance */
        distances.push_front((double *) calloc(sensorCount, sizeof(double)));
    };
    HCSR04.begin(trigPin, sensorPins, sensorCount);             /* Required by the library */
}

/* Class destructor */
DistanceSens::~DistanceSens() {}

/* Function that remains alive throughout the whole execution */
void DistanceSens::start(void)
{
    while(true){

        double *measures = (double *) calloc(sensorCount, sizeof(double)); /* Allocate memory for the new measures  */
        double *tmp = HCSR04.measureDistanceMm();                          /* Assign results to a temporary pointer */

        for(int i = 0; i < sensorCount; i++){
            measures[i] = tmp[i];                                          /* Copy the values into the new measures */
        }
        distances.push_front(measures);                                    /* Push the new measures into the list   */
        {                                                                  /* Bracket used to release the mutex     */
            std::unique_lock<std::mutex> lock(mtx);                        /* Enter critical section,               */
            for (int i = 0; i < sensorCount; i++)                          /* the averages mustn't be accessed      */
            {   
                averages[i] += distances.front()[i] / FILTER_SAMPLE_NUM;   
                averages[i] -= distances.back()[i] / FILTER_SAMPLE_NUM;
            }
        }
        distances.pop_back();
        delay(trigTimer);

    }
};

/* Function to set a new timer */
void DistanceSens::setTimer(uint16_t trigTimer)
{
    this->trigTimer = trigTimer;
}

/* Gets distance of the left sensor */
double DistanceSens::getDistanceL(void){
    std::unique_lock<std::mutex> lock(mtx);
    return averages[0];
};

/* Gets distance of the center sensor */
double DistanceSens::getDistanceC(void){
    std::unique_lock<std::mutex> lock(mtx);
    return averages[1];
};

/* Gets distance of the right sensor */
double DistanceSens::getDistanceR(void){
    std::unique_lock<std::mutex> lock(mtx);
    return averages[2];
};
