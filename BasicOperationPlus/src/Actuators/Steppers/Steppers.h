// Steppers.h

#ifndef STEPPERS_H
#define STEPPERS_H


#define PI 3.1415926535897932384626433832795 /* You know this                               */
#define WHEEL_DIAMETER 66.5                  /* Hardware property                           */
#define CIRCUMFERENCE  208.9159   /* Hardware property, calculated as: WHEEL_DIAMETER * PI  */ 
#define STEPS_PER_REVOLUTION 2048 /* Hardware property: specific to the BYJ28 stepper       */
#define STEP_PER_DISTANCE 10      /* Hardware property: Effectively 9.802                   */ 
#define G1  24      /* Number of steps to take to turn 1 degree                             */
#define G5  120     /* Number of steps to take to turn 5 degree                             */
#define G10 240     /* Number of steps to take to turn 10 degree                            */
#define G15 360     /* Number of steps to take to turn 15 degree                            */
#define G30 720     /* Number of steps to take to turn 30 degrees                           */
#define G45 1080    /* Number of steps to take to turn 45 degrees                           */
//#define G90 2160    /* Number of steps to take to turn 90 degrees                           */
#define G90 2180 /* Experimental value */

#include "Arduino.h"
#include <list> /* Utility library */
#include "../../Util/MySemaphore/MySemaphore.h" /* Synchronization library */

/* Class that implements a threadable system with two stepper motors that allow for simple movement */
class Steppers{
    public:
        /* Class constructor, requires the pins of each motor */
        Steppers(const uint8_t (&leftPins)[4], const uint8_t (&rightPins)[4]);

        /* Class destructor */
        ~Steppers();

        void start(void); /* Function starts the connection to the motors. Best utilized as a thread                */
        enum StepInstr {GoForwards, GoBackwards, TurnLeft, TurnRight}; /* Used to distinguish the instructions      */
        static StepInstr *uintToStepInstr(uint8_t num);/* Function to convert an integer into a stepper instruction, */
                                                /* returns NULL if value is not assignable                          */
        void goForwards (uint16_t millimeters); /* Function to move both motors forwards                                 */
        void goBackwards(uint16_t millimeters); /* Function to move both motors backwards                                */
        void turnLeft   (uint16_t degrees);     /* Function to turn to the left                                          */
        void turnRight  (uint16_t degrees);     /* Function to turn to the right                                         */ 
        /* Function to add an instruction to the tasks to perform, requires the task and the parameter for it */
        void addInstruction(StepInstr instr, uint16_t param);
    private:
        uint8_t _speed; /* Time to take between steps of the motor                                                      */
        std::list<StepInstr> _instructions; /* List of instructions to perform                                      */
        std::list<uint16_t> _params;             /* List of parameters to use                                            */
        MySemaphore *_instrCount;           /* Number of instructions available, if it is at 0 it blocks the thread */
        
        uint8_t _pinsSx[4]; /* Left motor pins  */
        uint8_t _pinsDx[4]; /* Right motor pins */

        /* Function to move forwards, acts directly on the motors, requires the amount of millimeters to move */
        void _goForwards (uint16_t millimeters);
        /* Function to move backwards, acts directly on the motors, requires the amount of millimeters to move */
        void _goBackwards(uint16_t millimeters);
        /* Function to turn left, acts directly on the motors, requires the amount of degrees to turn */
        void _turnLeft   (uint16_t degrees);
        /* Function to turn right, acts directly on the motors, requires the amount of degrees to turn */
        void _turnRight  (uint16_t degrees);
        /* Function to lower all pin output */
        void _lowPins(void);
        /* Function to turn millimeters into steps */
        uint16_t _mmToSteps(uint16_t millimeters);
        /* Function to turn degrees into steps */
        uint16_t _degreesToSteps(uint16_t degrees);
};

#endif