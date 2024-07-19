#include "Arduino.h"
#include <thread>
#include "../Pins.h"

/* Class for a threadable blinking LED */
class LEDBuiltIn{
    public:
        /* Class constructor */
        LEDBuiltIn(uint16_t speed);
        /* Threadable method to start blinking an LED */
        void startBlink(void);
        /* Method used to stop the LED if blinking */
        void stopBlink(void);
    private:
        bool _blinking;
        /* Blinking speed */
        uint16_t _speed;
};


