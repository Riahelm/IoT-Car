#include "Steppers.h"
#include "Arduino.h"

Steppers::Steppers(const int (&leftPins)[4], const int (&rightPins)[4]){

    this->_speed  = 2;
    _instrCount   = new MySemaphore(0);
    
    for (int i = 0; i < 4; i++)
    {
        _pinsSx[i] = leftPins[i];
        _pinsDx[i] = rightPins[i];

        pinMode(_pinsSx[i], OUTPUT);
        pinMode(_pinsDx[i], OUTPUT);

    }
    
};

Steppers::~Steppers(){};

void Steppers::start(){
    while(true){
        
        _instrCount->acquire();
        int param = _params.front();

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

void Steppers::goForwards(int millimeters) {
    this->_addInstruction(Steppers::GoForwards, millimeters);
};
void Steppers::goBackwards(int millimeters) {
    this->_addInstruction(Steppers::GoBackwards, millimeters);
};
void Steppers::turnLeft(int degrees) {
    this->_addInstruction(Steppers::TurnLeft, degrees);
};
void Steppers::turnRight(int degrees) {
    this->_addInstruction(Steppers::TurnRight, degrees);
};

void Steppers::_goForwards(int millimeters){
    int count;
    Serial.println(String(millimeters)); 
    Serial.println(String(_mmToSteps(millimeters))); 
    for (int i = 0; i < _mmToSteps(millimeters); i++){
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

void Steppers::_goBackwards(int millimeters){
    int count;
    for (int i = 0; i < _mmToSteps(millimeters); i++){
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

void Steppers::_turnLeft(int degrees){
    int count;
    for (int i = 0; i < _degreesToSteps(degrees)*2; i++){
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
void Steppers::_turnRight(int degrees){
    int count;
    for (int i = 0; i < _degreesToSteps(degrees)*2; i++){
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

void Steppers::_addInstruction(StepInstr instr, int param) {
    _instructions.push_back(instr);
    _params.push_back(param);
    _instrCount->release();
};



void Steppers::_lowPins(void){
    for (int i = 0; i < 4; i++){
        digitalWrite(_pinsSx[i], LOW);
        digitalWrite(_pinsDx[i], LOW);
    };
};

int Steppers::_mmToSteps(int millimeters) {
    return (millimeters * STEP_PER_DISTANCE);
};

int Steppers::_degreesToSteps(int degrees){
    return degrees * STEP_REV_IN_DG;
}