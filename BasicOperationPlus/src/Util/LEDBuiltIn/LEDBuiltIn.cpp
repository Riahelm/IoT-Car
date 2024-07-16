#include "LEDBuiltIn.h"

LEDBuiltIn::LEDBuiltIn(uint16_t speed)
{
    pinMode(LED_BUILTIN, OUTPUT);
    blinking = false;
    this->speed = speed;
}

void LEDBuiltIn::startBlink(void)
{
    blinking = true;
    while(blinking == true){
        digitalWrite(LED_BUILTIN, HIGH);
        delay(speed);
        digitalWrite(LED_BUILTIN, LOW);
        delay(speed);
    }
}

void LEDBuiltIn::stopBlink(void)
{
    blinking = false;
}
