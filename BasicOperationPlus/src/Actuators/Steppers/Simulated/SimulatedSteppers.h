
#ifndef SIMULATED_STEPPERS_H
#define SIMULATED_STEPPERS_H

#include "../Abstract/AbstractSteppers.h"
#include "../../../SimulationMap/SimulationMap.h"

class SimulatedSteppers : public AbstractSteppers{
    public:

        SimulatedSteppers(SimulationMap map);
    
    protected:

        SimulationMap map;

        /* Function to move forwards, acts directly on the motors, requires the amount of millimeters to move */
        virtual void _goForwards (uint16_t millimeters);
        /* Function to move backwards, acts directly on the motors, requires the amount of millimeters to move */
        virtual void _goBackwards(uint16_t millimeters);
        /* Function to turn left, acts directly on the motors, requires the amount of degrees to turn */
        virtual void _turnLeft   (uint16_t degrees);
        /* Function to turn right, acts directly on the motors, requires the amount of degrees to turn */
        virtual void _turnRight  (uint16_t degrees);
};

#endif