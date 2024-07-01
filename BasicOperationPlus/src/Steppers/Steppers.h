// Steppers.h

#ifndef STEPPERS_H
#define STEPPERS_H

#define WHEEL_DIAMETER 66.5
#define CIRCUMFERENCE  208.9159 // (WHEEL_DIAMETER * PI)
#define STEPS_PER_REVOLUTION 2048
#define MM_PER_STEP 1.0201 // (CIRCUMFERENCE / STEPS_PER_REVOLUTION)
#define G15 360
#define G30 720
#define G45 1080
#define G90 2160

#include <list>
#include "../MySemaphore/MySemaphore.h"

/*typedef enum StepperInstructions{
    GoForwards = 0,
    GoBackwards= 1,
    TurnLeft   = 2,
    TurnRight  = 3,
}StepperInstructions_t;*/

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