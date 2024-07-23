
#include "MyTelnet.h"

static ESPTelnet telnet;
static SimulationMap *telMap = nullptr;
static AbstractSteppers *telStep;
static DistanceSens *telSens;

/* Functions that are strictly to be used as callbacks for ESPTelnet */
namespace telnetCallbacks{
    /* Function to handle the event of a new connection                 */
    void onConnect(String);
    /* Function to handle the event of a connection attempt             */
    void onConnectionAttempt(String);
    /* Function to handle the event of a reconnection                   */
    void onReconnect(String);
    /* Function to handle the event of a disconnection                  */
    void onDisconnect(String);
    /* Function to handle the event of receiving a string in input      */
    void onInput(String);
}

/* Definition of a new type. The type is a non-returning function with a String for input */
typedef void (*func_to_call)(String param);

/* New type used to combine the name of a command to their respective parser */
typedef struct myFunc{
    String name;
    func_to_call fun;
} myFunc_t;

/* Custom functions that implement different ways to parse and execute a command */
namespace parsers{
    /* Function to parse the parameter used for going forward, input must be a natural number under 4000        */
    void GoForward(String);
    /* Function to parse the parameter used for going backward input must be a natural number under 4000        */
    void GoBackward(String);
    /* Function to parse the parameter used for turning to the left,  input must be a natural number under 360  */
    void TurnLeft(String);
    /* Function to parse the parameter used for turning to the right, input must be a natural number under 360  */
    void TurnRight(String);
    /* Function to print the distance value detected by the sensor to the left, no input required               */
    void PingLeft(String);
    /* Function to print the distance value detected by the sensor in the center, no input required             */
    void PingCenter(String);
    /* Function to print the distance value detected by the sensor to the right, no input required              */
    void PingRight(String);
    /* Function to print the virtual map, requires the virtualMap to be enabled */
    void PrintMap(String);
    /* Function to parse the parameter used for moving to a (X,Y) position, 
       input must not exceed boundaries set by the map area                                                     */
    void GoTo(String);
    /* Function to disconnect from the telnet server regardless of parameter usage                              */
    void Disconnect(String);
}

/* Array of the new type used to combine a command to their parsing function */
myFunc_t functions[NUM_OF_COMMANDS] = {
    {"goforward",   parsers::GoForward},
    {"goforwards",  parsers::GoForward},
    {"gobackward",  parsers::GoBackward},
    {"gobackwards", parsers::GoBackward},
    {"turnleft",    parsers::TurnLeft},
    {"turnright",   parsers::TurnRight},
    {"pingleft",    parsers::PingLeft},
    {"pingcenter",  parsers::PingCenter},
    {"pingright",   parsers::PingRight},
    {"printmap",    parsers::PrintMap},
    {"goto",        parsers::GoTo},
    {"disconnect",  parsers::Disconnect},
};

/* Function to print an error in case the requested command is unavailable */
void printError(String);


void setSteppers(AbstractSteppers *steppers)   {telStep = steppers;}
void setSensors (DistanceSens *sensors){telSens = sensors;}
void setMap(SimulationMap *map){telMap = map;}

void initTelnet(void){   

    /* Set all the callbacks */
    telnet.onConnect(telnetCallbacks::onConnect);
    telnet.onConnectionAttempt(telnetCallbacks::onConnectionAttempt);
    telnet.onReconnect(telnetCallbacks::onReconnect);
    telnet.onDisconnect(telnetCallbacks::onDisconnect);
    telnet.onInputReceived(telnetCallbacks::onInput);

    /* If telnet is unable to begin, restart the ESP (Telnet is mandatory) */
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
        /*  Forever maintain the telnet loop, occasionally going to 
            sleep to reduce the requests made to the CPU */
        telnet.loop();
        delay(THREAD_POLLING_INTERVAL);
    }
}


/* BEGIN CALLBACKS NAMESPACE */


void telnetCallbacks::onConnect(String ip){
    Serial.print("- Telnet: ");
    Serial.print(ip);
    Serial.println(" connected");
    telnet.println("\nWelcome " + telnet.getIP());
    telnet.print("> ");
}

void telnetCallbacks::onDisconnect(String ip) {
    Serial.print("- Telnet: ");
    Serial.print(ip);
    Serial.println(" disconnected");
}

void telnetCallbacks::onReconnect(String ip) {
    Serial.print("- Telnet: ");
    Serial.print(ip);
    Serial.println(" reconnected");
}

void telnetCallbacks::onConnectionAttempt(String ip) {
    Serial.print("- Telnet: ");
    Serial.print(ip);
    Serial.println(" tried to connect");
}

void telnetCallbacks::onInput(String str){
    int i = 0;
    /* Remove whitespace, \r and \n                                                     */
    str.trim(); 
    /* Turn everything in order to match it with the command table                      */
    str.toLowerCase(); 

    /* Split initial function into <Instruction, Parameter>                             */
    std::tuple<String, String> funPar = splitFunc(str); 

    std::get<1>(funPar).trim(); /* Remove whitespace, \r and \n from the parameter      */
    
    /* Look through the whole table or until able to match the requested function       */
    while(functions[i].name != std::get<0>(funPar) && i <= NUM_OF_COMMANDS){i++;}


    /* If able to match, execute function, if unable to match print an error,
       if input is empty do nothing */
    if (functions[i].name == std::get<0>(funPar)){
        telnet.println("Command succesfully matched and will soon execute, please wait.");
        functions[i].fun(std::get<1>(funPar));
    }else if(std::get<0>(funPar).length() != 0){
        printError(std::get<0>(funPar));
    }
    telnet.print("\n> ");
}

/* END CALLBACKS NAMESPACE */

/* BEGIN PARSERS NAMESPACE */

void parsers::GoForward(String param){
    /* Turn parameter into usable data */ 
    long normalParam = param.toInt(); 
    /* Perform parameter check */ 
    if(0 <= normalParam <= 4000){
        telStep->goForwards(normalParam);
    }else{
        telnet.println("Parameter error. Parameter must be a natural number lower than 4000.");
    }
};

void parsers::GoBackward(String param){
    /* Turn parameter into usable data */ 
    long normalParam = param.toInt();  
    /* Perform parameter check */
    if(0 <= normalParam <= 4000){ 
        telStep->goBackwards(normalParam);
    }else{
        telnet.println("Parameter error. Parameter must be a natural number lower than 4000.");
    }
}

void parsers::TurnLeft(String param){
    /* Turn parameter into usable data */ 
    long normalParam = param.toInt(); 
    /* Perform parameter check */
    if(0 <= normalParam <= 360){
        telStep->turnLeft(normalParam);
    }else{
        telnet.println("Parameter error. Parameter must be a natural number between 0 and 360.");
    }
}

void parsers::TurnRight(String param){
    /* Turn parameter into usable data */ 
    long normalParam = param.toInt(); 
    /* Perform parameter check */
    if(0 <= normalParam <= 360){
        telStep->turnRight(normalParam);
    }else{
        telnet.println("Parameter error. Parameter must be a natural number between 0 and 360.");
    }
}

void parsers::PingLeft(String param){
    /* Perform parameter check */
    if(param.length() == 0){
        telnet.println(telSens->getDistanceL());
    }
}

void parsers::PingCenter(String param){
    /* Perform parameter check */
    if(param.length() == 0){
        telnet.println(telSens->getDistanceC());
    }
}

void parsers::PingRight(String param){
    /* Perform parameter check */
    if(param.length() == 0){
        telnet.println(telSens->getDistanceR());
    }
}

 void parsers::PrintMap(String param){
    if(param.length() == 0 && telMap != nullptr){
        uint16_t len = 10;
        for (uint16_t i = 0; i < len; i++){
            for (uint16_t j = 0; j < len; i++){
                telnet.print(telMap->grid[i][j]);
            }
            telnet.println();
        }
    }
 }

void parsers::GoTo(String param){
    //TBD 
}

void parsers::Disconnect(String param){
    if (param.length() != 0){
        telnet.println("Disconnecting regardless of parameter...");
    }
    telnet.println("It has been a pleasure. ");
    telnet.disconnectClient();
}

/* END PARSERS NAMESPACE */

void printError(String instr){
    telnet.println("Input command was: " + instr + ", command unavailable, please give another input.");
};