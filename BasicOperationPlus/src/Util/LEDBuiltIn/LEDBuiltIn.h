#include "Arduino.h"
#include <thread>
#include "../Pins.h"
class LEDBuiltIn{
    public:
        LEDBuiltIn(uint16_t speed);
        void startBlink(void);
        void stopBlink(void);
    private:
        bool blinking;
        uint16_t speed;
};


