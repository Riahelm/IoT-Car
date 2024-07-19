
#include "MyTelnet.h"

static ESPTelnet telnet;
static Steppers *telStep;
static DistanceSens *telSens;

void parseGoForward(String param){
    long normalParam = param.toInt(); 
    if(0 <= normalParam <= 4000){
        telStep->goForwards(normalParam);
    }else{
        telnet.println("Parameter error. Parameter must be a natural number lower than 4000.");
    }
};

void parseGoBackward(String param){
    long normalParam = param.toInt(); 
    if(0 <= normalParam <= 4000){
        telStep->goBackwards(normalParam);
    }else{
        telnet.println("Parameter error. Parameter must be a natural number lower than 4000.");
    }
}
void parseTurnLeft(String param){
    long normalParam = param.toInt(); 
    if(0 < normalParam / 360 < 1){
        telStep->turnLeft(normalParam);
    }else{
        telnet.println("Parameter error. Parameter must be a natural number between 0 and 360.");
    }
}
void parseTurnRight(String param){
    long normalParam = param.toInt(); 
    if(0 < normalParam / 360 < 1){
        telStep->turnRight(normalParam);
    }else{
        telnet.println("Parameter error. Parameter must be a natural number between 0 and 360.");
    }
}
void parsePingLeft(String param){
    telnet.println("Param L: " + String(param.length()));
    if(param.length() == 0){
        telnet.println(telSens->getDistanceL());
    }
}
void parsePingCenter(String param){
    telnet.println("Param L: " + String(param.length()));
    if(param.length() == 0){
        telnet.println(telSens->getDistanceC());
    }
}
void parsePingRight(String param){
    telnet.println("Param L: " + String(param.length()));
    if(param.length() == 0){
        telnet.println(telSens->getDistanceR());
    }
}

void parseGoTo(String param){
    //TBD 
}

void parseDisconnect(String param){
    if (param.length() != 0){
        telnet.println("Disconnecting regardless of parameter...");
    }
    telnet.disconnectClient();
}

void printError(String instr){
    telnet.println("Input function was: " + instr + ", please give another input.");
};

typedef void (*func_to_call)(String param);

typedef struct myFunc{
    String name;
    func_to_call fun;
} myFunc_t;

myFunc_t functions[NUM_OF_COMMANDS] = {
    {"goforward",   parseGoForward},
    {"goforwards",  parseGoForward},
    {"gobackward",  parseGoBackward},
    {"gobackwards", parseGoBackward},
    {"turnleft",    parseTurnLeft},
    {"turnright",   parseTurnRight},
    {"pingleft",    parsePingLeft},
    {"pingcenter",  parsePingCenter},
    {"pingright",   parsePingRight},
    {"goto",        parseGoTo},
    {"disconnect",  parseDisconnect},
    {"error",       printError},
};

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

void onTelnetInput(String str){
    str.trim();
    str.toLowerCase();

    Serial.println(str);

    std::tuple<String, String> funPar = splitFunc(str);

    int i = 0;
    std::get<1>(funPar).trim();
    
    while(i < NUM_OF_COMMANDS && functions[i].name != std::get<0>(funPar)){i++;}

    telnet.println();


    if (functions[i].name == std::get<0>(funPar)){
        functions[i].fun(std::get<1>(funPar));
    }else if(std::get<0>(funPar).length() != 0){
        printError(std::get<0>(funPar));
    }
    telnet.print("> ");
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
