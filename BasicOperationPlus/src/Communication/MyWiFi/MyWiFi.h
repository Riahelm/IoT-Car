#ifndef MYWIFI_H
#define MYWIFI_H

#include "../Util/Pins.h"
#include "../Util/LEDBuiltIn.h"
#include <thread>
#include <WiFi.h>

#define WIFI_SSID "Nicolas' Galaxy Z Fold2 5G"
#define WIFI_PASSWORD "Succ1234"

void setupWiFi(void);
bool isConnected(void);
void printValues();

#endif //MyWiFi.h