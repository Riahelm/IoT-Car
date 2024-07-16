#include "MyWiFi.h"

void setupWiFi(void){
    LEDBuiltIn led(250);
    std::thread builtIn(&LEDBuiltIn::startBlink, &led);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("Connecting to wifi");

    Serial.print("ESP Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
    Serial.println("ESP Board name:  " + String(WiFi.getHostname()));
    

    uint8_t i = 0;
    while((WiFi.status() != WL_CONNECTED) && i < 10){
        delay(500);
        Serial.print(".");
        i++;
    }

    if(i >= 10){
        Serial.println("Connection timed out");
    }else{
        IPAddress ip = WiFi.localIP();
        Serial.println("Connection successful");
        Serial.print("ESP IP: ");
        Serial.println(WiFi.localIP().toString());
    }
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    led.stopBlink();
    builtIn.join();
}

bool isConnected(void)
{
    return (WiFi.status() == WL_CONNECTED);
}

void printValues(void){
    int i = 0;
    while(i < 10);
        Serial.println("Signal strength: " + String(WiFi.RSSI()));
}


