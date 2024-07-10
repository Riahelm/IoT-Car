// Steppers.h

#ifndef STEPPERS_H
#define STEPPERS_H

#define WHEEL_DIAMETER 66.5
#define CIRCUMFERENCE  208.9159 // (WHEEL_DIAMETER * PI) 
#define STEPS_PER_REVOLUTION 2048
#define STEP_PER_DISTANCE 9.802 //In steps
#define PI 3.1415926535897932384626433832795
#define FULL_STEP_ANGLE 0.176
#define G15 360
#define G30 720
#define G45 1080
#define G90 2160

#include <list>
#include "../MySemaphore/MySemaphore.h"
class Steppers{
    public:
        Steppers(const int (&leftPins)[4], const int (&rightPins)[4]);
        ~Steppers();
        void start(void);
        enum StepInstr {GoForwards, GoBackwards, TurnLeft, TurnRight};
        void goForwards (int millimeters);
        void goBackwards(int millimeters);
        void turnLeft   (int degrees);
        void turnRight  (int degrees);
    private:
        int _speed;
        std::list<StepInstr> _instructions;
        std::list<int> _params;
        MySemaphore *_instrCount;
        
        int _pinsSx[4];
        int _pinsDx[4];
    
        void _addInstruction(StepInstr instr, int param);
        void _goForwards (int millimeters);
        void _goBackwards(int millimeters);
        void _turnLeft   (int degrees);
        void _turnRight  (int degrees);
        void _lowPins(void);
        int _mmToSteps(int millimeters);
};

#endif