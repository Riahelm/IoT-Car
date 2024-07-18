#include "Steppers.h" 

/* Class constructor */
Steppers::Steppers(const uint8_t (&leftPins)[4], const uint8_t (&rightPins)[4]){

    this->_speed  = 2;  /* Default speed is 2, which is the minimum for a BYJ stepper                           */
    _instrCount   = new MySemaphore(0); /* Starting amount of instructions is zero                              */
    
    for (uint8_t i = 0; i < 4; i++) /* Assign all the pins to the respective variables and puts them in output mode */
    {
        _pinsSx[i] = leftPins[i];
        _pinsDx[i] = rightPins[i];

        pinMode(_pinsSx[i], OUTPUT);
        pinMode(_pinsDx[i], OUTPUT);

    }
    
};

/* Class destructor */
Steppers::~Steppers(){};

/* Function to start reading and executing commands in the instruction list. Best used as a thread.*/
void Steppers::start(){
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

Steppers::StepInstr *Steppers::uintToStepInstr(uint8_t num)
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
void Steppers::goForwards(uint16_t millimeters) {
    this->addInstruction(Steppers::GoForwards, millimeters);
};

/* Function callable by another thread, adds an instruction */
void Steppers::goBackwards(uint16_t millimeters) {
    this->addInstruction(Steppers::GoBackwards, millimeters);
};

/* Function callable by another thread, adds an instruction */
void Steppers::turnLeft(uint16_t degrees) {
    this->addInstruction(Steppers::TurnLeft, degrees);
};

/* Function callable by another thread, adds an instruction */
void Steppers::turnRight(uint16_t degrees) {
    this->addInstruction(Steppers::TurnRight, degrees);
};

/* Function that isn't to be called by another thread, actually acts on the motors */
void Steppers::_goForwards(uint16_t millimeters){
    uint8_t count;
    
    for (uint16_t i = 0; i < _mmToSteps(millimeters); i++){
        count = i % 4;
        digitalWrite(_pinsSx[count], HIGH);
        digitalWrite(_pinsDx[count], HIGH);
        delay(this->_speed);
        digitalWrite(_pinsSx[count], LOW);
        digitalWrite(_pinsDx[count], LOW);
        delay(this->_speed);
    };

    _lowPins();
};

/* Function that isn't to be called by another thread, actually acts on the motors */
void Steppers::_goBackwards(uint16_t millimeters){
    uint8_t count;
    for (uint16_t i = 0; i < _mmToSteps(millimeters); i++){
        count = i % 4;
        digitalWrite(_pinsSx[3 - count], HIGH);
        digitalWrite(_pinsDx[3 - count], HIGH);
        delay(this->_speed);
        digitalWrite(_pinsSx[3 - count], LOW);
        digitalWrite(_pinsDx[3 - count], LOW);
        delay(this->_speed);
    };

    _lowPins();
};

/* Function that isn't to be called by another thread, actually acts on the motors */
void Steppers::_turnLeft(uint16_t degrees){
    uint8_t count;
    for (uint16_t i = 0; i < _degreesToSteps(degrees)/2; i++){
        count = i % 4;
        digitalWrite(_pinsSx[3 - count], HIGH);
        digitalWrite(_pinsDx[count], HIGH);
        delay(this->_speed);
        digitalWrite(_pinsSx[3 - count], LOW);
        digitalWrite(_pinsDx[count], LOW);
        delay(this->_speed);
    };

    _lowPins();
};

/* Function that isn't to be called by another thread, actually acts on the motors */
void Steppers::_turnRight(uint16_t degrees){
    uint8_t count;
    for (uint16_t i = 0; i < _degreesToSteps(degrees)/2; i++){
        count = i % 4;
        digitalWrite(_pinsSx[count], HIGH);
        digitalWrite(_pinsDx[3 - count], HIGH);
        delay(this->_speed);
        digitalWrite(_pinsSx[count], LOW);
        digitalWrite(_pinsDx[3 - count], LOW);
        delay(this->_speed);
    };

    _lowPins();
};

/* Wrapper function to ease adding instructions */
void Steppers::addInstruction(StepInstr instr, uint16_t param) {
    _instructions.push_back(instr);
    _params.push_back(param);
    _instrCount->release();
};

/* Utility function to lower all pins */
void Steppers::_lowPins(void){
    for (uint8_t i = 0; i < 4; i++){
        digitalWrite(_pinsSx[i], LOW);
        digitalWrite(_pinsDx[i], LOW);
    };
};

/* Utility function to turn millimeters into steps */
uint16_t Steppers::_mmToSteps(uint16_t millimeters) {
    return (millimeters * STEP_PER_DISTANCE);
};

/* Utility function to turn degrees into steps */
uint16_t Steppers::_degreesToSteps(uint16_t degrees){
    if(degrees == 90){
        return G90;
    } 
    return degrees * G1;
}