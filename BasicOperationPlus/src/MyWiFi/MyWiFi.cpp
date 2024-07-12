#include "MyWiFi.h"

void initWiFi(void){
    LEDBuiltIn led(250);
    std::thread builtIn(&LEDBuiltIn::startBlink, &led);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("Connecting to wifi");

    Serial.print("ESP Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
    Serial.print("ESP Board name:  " + String(WiFi.getHostname()));

    uint8_t i = 0;
    while((WiFi.status() != WL_CONNECTED) && i < 10){
        Serial.println("Connecting...");
        Serial.println();
        delay(2000);
        i++;
    };

    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);


}

void printValues(void){
    int i = 0;
    while(i < 10);
        Serial.println("Signal strength: " + String(WiFi.RSSI()));
}
