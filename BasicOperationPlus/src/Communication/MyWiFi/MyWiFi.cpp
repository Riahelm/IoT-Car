#include "MyWiFi.h"

void setupWiFi(String ssid, String password){
    /* Start blinking a LED to show device is connecting */
    LEDBuiltIn led(250);
    std::thread builtIn(&LEDBuiltIn::startBlink, &led);

    /* Connect in STATION mode with given, or default data */
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("Connecting to wifi");

    Serial.print("ESP Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
    Serial.println("ESP Board name:  " + String(WiFi.getHostname()));
    


    /* Try connection up to 10 times */
    uint8_t i = 0;
    while((WiFi.status() != WL_CONNECTED) && i < 10){
        delay(500);
        Serial.print(".");
        i++;
    }

    /* Print IP (Default IP assignment uses ARP) if connection was succesfull */
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

    /* Stop blinking in order to show connection either timed out or was succesfull */
    led.stopBlink();
    builtIn.join();
}

bool isConnected(void)
{
    return (WiFi.status() == WL_CONNECTED);
}