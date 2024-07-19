#include "LEDBuiltIn.h"

LEDBuiltIn::LEDBuiltIn(uint16_t speed)
{
    pinMode(LED_BUILTIN, OUTPUT);
    _blinking = false;
    this->_speed = speed;
}

void LEDBuiltIn::startBlink(void)
{
    _blinking = true;
    while(_blinking == true){
        digitalWrite(LED_BUILTIN, HIGH);
        delay(_speed);
        digitalWrite(LED_BUILTIN, LOW);
        delay(_speed);
    }
}

void LEDBuiltIn::stopBlink(void)
{
    _blinking = false;
}
