// Steppers.h

#ifndef PHYSICAL_STEPPERS_H
#define PHYSICAL_STEPPERS_H

#include "../Abstract/AbstractSteppers.h"

/* Class that implements a threadable system with two stepper motors that allow for simple movement */
class PhysicalSteppers : public AbstractSteppers{
    public:
        /* Class constructor, requires the pins of each motor */
        PhysicalSteppers(const uint8_t (&leftPins)[4], const uint8_t (&rightPins)[4]);

        ~PhysicalSteppers(); 

    protected:

        uint8_t _speed; /* Time to take between steps of the motor                                                      */
        
        uint8_t _pinsSx[4]; /* Left motor pins  */
        uint8_t _pinsDx[4]; /* Right motor pins */

        /* Function to move forwards, acts directly on the motors, requires the amount of millimeters to move */
        virtual void _goForwards (uint16_t millimeters);
        /* Function to move backwards, acts directly on the motors, requires the amount of millimeters to move */
        virtual void _goBackwards(uint16_t millimeters);
        /* Function to turn left, acts directly on the motors, requires the amount of degrees to turn */
        virtual void _turnLeft   (uint16_t degrees);
        /* Function to turn right, acts directly on the motors, requires the amount of degrees to turn */
        virtual void _turnRight  (uint16_t degrees);

        /* Function to lower all pin output */
        void _lowPins(void);
        /* Function to turn millimeters into steps */
        uint16_t _mmToSteps(uint16_t millimeters);
        /* Function to turn degrees into steps */
        uint16_t _degreesToSteps(uint16_t degrees);
};

#endif