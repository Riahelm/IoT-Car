#ifndef MYWIFI_H
#define MYWIFI_H

#include "../Util/Pins.h"
#include "../Util/LEDBuiltIn.h"
#include <thread>
#include <WiFi.h>

#define WIFI_SSID "Nicolas' Galaxy Z Fold2 5G"
#define WIFI_PASSWORD "Succ1234"

void initWiFi(void);

void printValues();

#endif //MyWiFi.h