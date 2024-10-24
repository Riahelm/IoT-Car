#include "AbstractSteppers.h"

/* Function to start reading and executing commands in the instruction list. Best used as a thread.*/
void AbstractSteppers::start(){
    while(true){
        
        _instrCount->acquire();
        uint16_t param = _params.front();

        switch(_instructions.front()){
            case GoForwards:
                _goForwards(param);
                break;
            case GoBackwards:
                _goBackwards(param);
                break;
            case TurnLeft:
                _turnLeft(param);
                break;
            case TurnRight:
                _turnRight(param);
                break;
        }

        _instructions.pop_front();
        _params.pop_front();
    }
}

bool AbstractSteppers::isFinished(void)
{
    return this->_instructions.size() == 0;
}

/* Function to convert a number to the respective stepper instruction, returns nullptr if num isn't convertible */
AbstractSteppers::StepInstr *AbstractSteppers::uintToStepInstr(uint8_t num)
{   
    StepInstr *res;
    switch(num){
        case 0:
            *res = GoForwards;
            break;
        case 1:
            *res = GoBackwards;
            break;
        case 2:
            *res = TurnLeft;
            break;
        case 3:
            *res = TurnRight;
            break;
    }
    Serial.println("Num: " + String(num));
    Serial.println("Instr: " + String(*res));
    return (0 <= num <= 3 ? res: nullptr);
}

/* Function callable by another thread, adds an instruction */
void AbstractSteppers::goForwards(uint16_t millimeters) {
    this->_addInstruction(AbstractSteppers::GoForwards, millimeters);
};

/* Function callable by another thread, adds an instruction */
void AbstractSteppers::goBackwards(uint16_t millimeters) {
    this->_addInstruction(AbstractSteppers::GoBackwards, millimeters);
};

/* Function callable by another thread, adds an instruction */
void AbstractSteppers::turnLeft(uint16_t degrees) {
    this->_addInstruction(AbstractSteppers::TurnLeft, degrees);
};

/* Function callable by another thread, adds an instruction */
void AbstractSteppers::turnRight(uint16_t degrees) {
    this->_addInstruction(AbstractSteppers::TurnRight, degrees);
};

/* Wrapper function to ease adding instructions */
void AbstractSteppers::_addInstruction(StepInstr instr, uint16_t param) {
    _instructions.push_back(instr);
    _params.push_back(param);
    _instrCount->release();
};