#include "DistanceSens.h"

/* Class constructor */
DistanceSens::DistanceSens(uint8_t trigPin, uint8_t sensorCount, uint8_t *sensorPins, uint16_t trigTimer)
{
    this->_trigTimer = trigTimer;
    this->_sensorCount = sensorCount;
    /* Averages is an array holding each result */ 
    _averages = (double *) calloc(_sensorCount, sizeof(double)); 
    /* Initialize the average to 0 */ 
    for (int i = 0; i < _sensorCount; i++){_averages[i] = 0;};    
    /* Assign a memory location to each distance */
    for (int i = 0; i < FILTER_SAMPLE_NUM; i++){                
        _distances.push_front((double *) calloc(_sensorCount, sizeof(double)));
    };
    /* Required by the library */
    HCSR04.begin(trigPin, sensorPins, _sensorCount);             
}

/* Class destructor */
/* Remove averages, calloc present!! */
DistanceSens::~DistanceSens() {}

/* Function that remains alive throughout the whole execution */
void DistanceSens::start(void)
{
    while(true){

        double *measures = (double *) calloc(_sensorCount, sizeof(double)); /* Allocate memory for the new measures  */
        double *tmp = HCSR04.measureDistanceMm();                          /* Assign results to a temporary pointer */

        for(int i = 0; i < _sensorCount; i++){
            measures[i] = tmp[i];                                          /* Copy the values into the new measures */
        }
        _distances.push_front(measures);                                    /* Push the new measures into the list   */
        {                                                                  /* Bracket used to release the mutex     */
            std::unique_lock<std::mutex> lock(_mtx);                        /* Enter critical section,               */
            for (int i = 0; i < _sensorCount; i++)                          /* the averages mustn't be accessed      */
            {   
                _averages[i] += _distances.front()[i] / FILTER_SAMPLE_NUM;   
                _averages[i] -= _distances.back()[i] / FILTER_SAMPLE_NUM;
            }
        }
        _distances.pop_back();
        delay(_trigTimer);

    }
};

/* Function to set a new timer */
void DistanceSens::setTimer(uint16_t trigTimer)
{
    this->_trigTimer = trigTimer;
}

/* Gets distance of the left sensor */
double DistanceSens::getDistanceL(void){
    std::unique_lock<std::mutex> lock(_mtx);
    return _averages[0];
};

/* Gets distance of the center sensor */
double DistanceSens::getDistanceC(void){
    std::unique_lock<std::mutex> lock(_mtx);
    return _averages[1];
};

/* Gets distance of the right sensor */
double DistanceSens::getDistanceR(void){
    std::unique_lock<std::mutex> lock(_mtx);
    return _averages[2];
};
