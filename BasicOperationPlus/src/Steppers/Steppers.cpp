#include "Steppers.h"
#include "Arduino.h"
Steppers::Steppers(const int (&leftPins)[4], const int (&rightPins)[4]){

    this->speed = 2;

    for (int i = 0; i < 4; i++)
    {
        pinsSx[i] = leftPins[i];
        pinsDx[i] = rightPins[i];

        pinMode(pinsSx[i], OUTPUT);
        pinMode(pinsDx[i], OUTPUT);

        Serial.println("Turned on: " + String(pinsSx[i]));
        Serial.println("Turned on: " + String(pinsDx[i]));
    }
    
};

Steppers::~Steppers(){};

Steppers::start(){
    while(true){
        
        get<1>(instructions).acquire();
        auto instrAndParam = get<0>(instructions);
        auto instr = get<0>(instrAndParam);
        auto param = get<1>(instrAndParam);
        
        switch(instr){
            case GoForwards:
                goForwards(param);
                break;
            case GoBackwards:
                goBackwards(param);
                break;
            case TurnLeft:
                turnLeft(param);
                break;
            case TurnRight:
                turnRight(param);
                break;
        }
    }
}

void Steppers::goForwards(int millimeters){
    int count;
    for (int i = 0; i < mmToSteps(millimeters); i++){
        count = i % 4;
        digitalWrite(pinsSx[count], HIGH);
        digitalWrite(pinsDx[count], HIGH);
        Serial.println("Flipped: " + String(pinsSx[i]));
        Serial.println("Flipped: " + String(pinsDx[i]));
        delay(this->speed);
        digitalWrite(pinsSx[count], LOW);
        digitalWrite(pinsDx[count], LOW);
        delay(this->speed);
    };

    lowPins();
};

void Steppers::goBackwards(int millimeters){
    int count;
    for (int i = mmToSteps(millimeters); i > 0; i++){
        count = i % 4;
        digitalWrite(pinsSx[count], HIGH);
        digitalWrite(pinsDx[count], HIGH);
        delay(this->speed);
        digitalWrite(pinsSx[count], LOW);
        digitalWrite(pinsDx[count], LOW);
        delay(this->speed);
    };

    lowPins();
};

void Steppers::turnLeft(int millimeters){
    int count;
    for (int i = 0; i < mmToSteps(millimeters); i++){
        count = i % 4;
        digitalWrite(pinsSx[3 - count], HIGH);
        digitalWrite(pinsDx[count], HIGH);
        delay(this->speed);
        digitalWrite(pinsSx[3 - count], LOW);
        digitalWrite(pinsDx[count], LOW);
        delay(this->speed);
    };

    lowPins();
};
void Steppers::turnRight(int millimeters){
    int count;
    for (int i = 0; i < mmToSteps(millimeters); i++){
        count = i % 4;
        digitalWrite(pinsSx[count], HIGH);
        digitalWrite(pinsDx[3 - count], HIGH);
        delay(this->speed);
        digitalWrite(pinsSx[count], LOW);
        digitalWrite(pinsDx[3 - count], LOW);
        delay(this->speed);
    };

    lowPins();
}

void Steppers::addInstruction(tuple<StepperInstructions, int> instrAndParam) {
    
    get<0>(instructions).push_back(instrAndParam);
    get<1>(instructions).release();
};

void Steppers::lowPins(void){
    for (int i = 0; i < 4; i++){
        digitalWrite(pinsSx[i], LOW);
        digitalWrite(pinsDx[i], LOW);
    };
}

int Steppers::mmToSteps(int millimeters) {
     return (millimeters/MM_PER_STEP); 
}