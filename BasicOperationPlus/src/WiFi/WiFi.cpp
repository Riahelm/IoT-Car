#include "WiFi.h"

void initWiFi(void){
    LEDBuiltIn led(20);
    std::thread builtIn(&LEDBuiltIn::startBlink, &led);
    delay(2000);
}