#include "Arduino.h"
#include "../Util/Pins.h"
#include <thread>
class LEDBuiltIn{
    public:
        LEDBuiltIn(uint16_t speed);
        void startBlink(void);
        void stopBlink(void);
    private:
        bool blinking;
        uint16_t speed;
};


