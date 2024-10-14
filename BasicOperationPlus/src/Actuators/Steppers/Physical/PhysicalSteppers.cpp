#include "PhysicalSteppers.h" 

/* Class constructor */
PhysicalSteppers::PhysicalSteppers(const uint8_t (&leftPins)[4], const uint8_t (&rightPins)[4]){

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

PhysicalSteppers::~PhysicalSteppers(){};

/* Function that isn't to be called by another thread, actually acts on the motors */
void PhysicalSteppers::_goForwards(uint16_t millimeters){
    uint8_t count;
    uint16_t bound = _mmToSteps(millimeters);
    for (uint16_t i = 0; i < bound; i++){
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
void PhysicalSteppers::_goBackwards(uint16_t millimeters){
    uint8_t count;
    uint16_t bound = _mmToSteps(millimeters);
    for (uint16_t i = 0; i < bound; i++){
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
void PhysicalSteppers::_turnLeft(uint16_t degrees){
    uint8_t count;
    uint16_t bound = _degreesToSteps(degrees)/2;
    for (uint16_t i = 0; i < bound; i++){
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
void PhysicalSteppers::_turnRight(uint16_t degrees){
    uint8_t count;
    uint16_t bound = _degreesToSteps(degrees)/2;
    for (uint16_t i = 0; i < bound; i++){
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

/* Utility function to lower all pins */
void PhysicalSteppers::_lowPins(void){
    for (uint8_t i = 0; i < 4; i++){
        digitalWrite(_pinsSx[i], LOW);
        digitalWrite(_pinsDx[i], LOW);
    };
};

/* Utility function to turn millimeters into steps */
uint16_t PhysicalSteppers::_mmToSteps(uint16_t millimeters) {
    return (millimeters * STEP_PER_DISTANCE);
};

/* Utility function to turn degrees into steps */
uint16_t PhysicalSteppers::_degreesToSteps(uint16_t degrees){
    std::array<uint8_t, 8> knownSteps    = {1,5,10,15,30,45,90,180}; 
    std::array<uint16_t, 8> knownDegrees = {G1,G5,G10,G15,G30,G45,G90,G180};
    uint8_t i = knownSteps.size() - 1;
    uint16_t steps = 0;

    if(degrees == 0){
        return 0;
    }

    do{
        while(knownSteps[i] > degrees){i--;}
        degrees -= knownSteps[i];
        steps += knownDegrees[i];
    }while(degrees != 0);

    return steps;
}