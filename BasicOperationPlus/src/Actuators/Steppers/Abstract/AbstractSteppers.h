
#ifndef ABSTRACT_STEPPERS_H
#define ABSTRACT_STEPPERS_H

#include "Arduino.h"
#include <list> /* Utility library */
#include "../../../Util/Arrays/Arrays.h"           /* Utility library */
#include "../../../Util/MySemaphore/MySemaphore.h" /* Synchronization library */

#define PI 3.1415926535897932384626433832795 /* You know this                               */
#define WHEEL_DIAMETER 66.5                  /* Hardware property                           */
#define CIRCUMFERENCE  208.9159   /* Hardware property, calculated as: WHEEL_DIAMETER * PI  */ 
#define STEPS_PER_REVOLUTION 2048 /* Hardware property: specific to the BYJ28 stepper       */
#define STEP_PER_DISTANCE 10      /* Hardware property: Effectively 9.802                   */ 
#define G1  24      /* Number of steps to take to turn 1 degree                             */
#define G5  121     /* Number of steps to take to turn 5 degree                             */
#define G10 243     /* Number of steps to take to turn 10 degree                            */
#define G15 364     /* Number of steps to take to turn 15 degree                            */
#define G30 729     /* Number of steps to take to turn 30 degrees                           */
#define G45 1094    /* Number of steps to take to turn 45 degrees                           */
#define G90 2188    /* Number of steps to take to turn 90 degrees                           */
#define G180 4376   /* Number of steps to take to turn 180 degrees                          */

/* Class that implements a threadable system with two stepper motors that allow for simple movement */
class AbstractSteppers{
    public:

        /* Function starts the connection to the motors. Best utilized as a thread                    */
        void start(void); 
        /* Used to distinguish the instructions          */
        enum StepInstr {GoForwards, GoBackwards, TurnLeft, TurnRight}; 
        /* Function to convert an integer into a stepper instruction, returns NULL if value is not assignable */
        static StepInstr *uintToStepInstr(uint8_t num); 
        /* Function to move both motors forwards            */
        void goForwards (uint16_t millimeters);         
        /* Function to move both motors backwards           */
        void goBackwards(uint16_t millimeters);         
        /* Function to turn to the left                     */
        void turnLeft   (uint16_t degrees);             
        /* Function to turn to the right                    */ 
        void turnRight  (uint16_t degrees);             

    protected:
        std::list<StepInstr> _instructions; /* List of instructions to perform                                          */
        std::list<uint16_t> _params;        /* List of parameters to use                                                */
        MySemaphore *_instrCount;           /* Number of instructions available, if it is at 0 it blocks the thread     */

        /* Function to move forwards, acts directly on the motors, requires the amount of millimeters to move */
        virtual void _goForwards (uint16_t millimeters) = 0;
        /* Function to move backwards, acts directly on the motors, requires the amount of millimeters to move */
        virtual void _goBackwards(uint16_t millimeters) = 0;
        /* Function to turn left, acts directly on the motors, requires the amount of degrees to turn */
        virtual void _turnLeft   (uint16_t degrees) = 0;
        /* Function to turn right, acts directly on the motors, requires the amount of degrees to turn */
        virtual void _turnRight  (uint16_t degrees) = 0;
        /* Function to add an instruction to the tasks to perform, requires the task and the parameter for it */
        void _addInstruction(StepInstr instr, uint16_t param);
};

#endif