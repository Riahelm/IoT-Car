#include "EmergencyReset.h"

void emergencyReset(){
    esp_sleep_enable_timer_wakeup(5000000); //5s
    esp_deep_sleep_start();  
}
