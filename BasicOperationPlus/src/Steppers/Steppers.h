// Steppers.h

#ifndef STEPPERS_H
#define STEPPERS_H

#define WHEEL_DIAMETER 66.5
#define CIRCUMFERENCE  208.9159 // (WHEEL_DIAMETER * PI)
#define DISTANCE_PER_STEP 
#define STEPS_PER_REVOLUTION 2048
#define DISTANCE_PER_STEP 0,10213666782670816684157438377195 //(PI*WHEEL_DIAMETER*FULL_STEP_ANGLE) / 360
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
        void addInstruction(StepInstr instr, int param);
    private:
        int speed;
        std::list<StepInstr> instructions;
        std::list<int> params;
        MySemaphore *instrCount;
        
        int pinsSx[4];
        int pinsDx[4];
    
        void goForwards (int millimeters);
        void goBackwards(int millimeters);
        void turnLeft   (int degrees);
        void turnRight  (int degrees);
        void lowPins(void);
        int mmToSteps(int millimeters);
};

#endif