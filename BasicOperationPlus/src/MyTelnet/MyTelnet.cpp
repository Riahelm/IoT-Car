#include "MyTelnet.h"

void MyTelnet::onTelnetConnect(String ip){
    Serial.print("- Telnet: ");
    Serial.print(ip);
    Serial.println(" connected");
    telnet.println("\nWelcome " + telnet.getIP());
    telnet.println("(Use ^] + q  to disconnect.)");
}

void MyTelnet::onTelnetDisconnect(String ip) {
    Serial.print("- Telnet: ");
    Serial.print(ip);
    Serial.println(" disconnected");
}

void MyTelnet::onTelnetReconnect(String ip) {
    Serial.print("- Telnet: ");
    Serial.print(ip);
    Serial.println(" reconnected");
}

void MyTelnet::onTelnetConnectionAttempt(String ip) {
    Serial.print("- Telnet: ");
    Serial.print(ip);
    Serial.println(" tried to connect");
}

void MyTelnet::onTelnetInput(String str) {
// checks for a certain command
    if (str == "ping") {
        telnet.println("> pong"); 
        Serial.println("- Telnet: pong");
    // disconnect the client
    } else if (str == "bye") {
        telnet.println("> disconnecting you...");
        telnet.disconnectClient();
    }
}

void MyTelnet::MyTelnet(void){   
    telnet.onConnect(onTelnetConnect);
    telnet.onConnectionAttempt(onTelnetConnectionAttempt);
    telnet.onReconnect(onTelnetReconnect);
    telnet.onDisconnect(onTelnetDisconnect);
    telnet.onInputReceived(onTelnetInput);

    Serial.print("- Telnet: ");
    if (telnet.begin(TELNET_PORT)) {
        Serial.println("running");
    } else {
        Serial.println("error. Restarting.");
        ESP.restart();
    }
}

void MyTelnet::startTelnet(void)
{
    while(true){
        telnet.loop();
    }
}
