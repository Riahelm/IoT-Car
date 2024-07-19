#ifndef MYWIFI_H
#define MYWIFI_H

#include <thread>
#include <WiFi.h>
#include "../../Util/Pins.h"
#include "../../Util/LEDBuiltIn/LEDBuiltIn.h"

/* Network information   */
#define DEFAULT_WIFI_SSID "Nicolas' Galaxy Z Fold2 5G"
/* Sensitive information */
#define DEFAULT_WIFI_PASSWORD "Succ1234"

/* Function used to connect to the default network */
void setupWiFi(String = DEFAULT_WIFI_SSID, String = DEFAULT_WIFI_PASSWORD);   
/* Function used to connect to a specific network */
void setupWiFi(String, String);    
/* Function used to check if device is connected to a network */
bool isConnected(void);

#endif //MyWiFi.h