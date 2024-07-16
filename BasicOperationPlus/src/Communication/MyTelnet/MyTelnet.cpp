#include "MyTelnet.h"

static ESPTelnet telnet;
static Steppers *telStep;
static DistanceSens *telSens;

static void onTelnetConnect(String ip){
    Serial.print("- Telnet: ");
    Serial.print(ip);
    Serial.println(" connected");
    telnet.println("\nWelcome " + telnet.getIP());
    telnet.print("> ");
}

static void onTelnetDisconnect(String ip) {
    Serial.print("- Telnet: ");
    Serial.print(ip);
    Serial.println(" disconnected");
}

static void onTelnetReconnect(String ip) {
    Serial.print("- Telnet: ");
    Serial.print(ip);
    Serial.println(" reconnected");
}

static void onTelnetConnectionAttempt(String ip) {
    Serial.print("- Telnet: ");
    Serial.print(ip);
    Serial.println(" tried to connect");
}

static void onTelnetInput(String str) {
    /*if(valid(str)){               //PSEUDO CODE
        extractInstruction(str);
        extractParameter(str);
        executeFunc(instr, param);
    }else{
        telnet.println("Spelling error. Please check input.");
    }*/
 //checks for a certain command
    if (str == "ping") {
        telnet.println("> pong"); 
        Serial.println("- Telnet: pong");
        telnet.print("> ");
        telnet.println(String(telSens->getDistanceC()));
        telnet.print("> ");
    // disconnect the client
    } else if (str == "bye") {
        telnet.println("> disconnecting you...");
        telnet.disconnectClient();
    } else if (valid(str)){
        telnet.println("I read this line: " + str);
        telStep->goForwards(200);
    }
}

void initTelnet(void){   

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

void startTelnet(void)
{
    while(true){
        telnet.loop();
        delay(THREAD_POLLING_INTERVAL);
    }
}

void setMotors(Steppers *steppers)
{
    telStep = steppers;
}

void setSensors(DistanceSens *sensors)
{
    telSens = sensors;
}
