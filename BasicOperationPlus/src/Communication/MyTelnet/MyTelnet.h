#ifndef MYTELNET_H
#define MYTELNET_H

#include <tuple>
#include <ESPTelnet.h>
#include "../MyWiFi/MyWiFi.h"
#include "../../Actuators/Steppers/Abstract/AbstractSteppers.h"
#include "../../Actuators/Steppers/Simulated/SimulatedSteppers.h"
#include "../../Actuators/Steppers/Physical/PhysicalSteppers.h"
#include "../../Sensors/Distance/DistanceSens.h"
#include "../../Util/StringParser/StringParser.h"
#include "../../Util/Arrays/Arrays.h"

/* Port through which the socket is defined. Usually either 23 or 2323 */
#define TELNET_PORT 23 
/* Time spent between each iteration of the telnet loop. Affects response times and CPU usage */
#define THREAD_POLLING_INTERVAL 20
/* Number of defined terminal commands */
#define NUM_OF_COMMANDS 12

/* Function required to enable the steppers module */
void setSteppers(AbstractSteppers *steppers);
/* Function required to enable the sensors module  */
void setSensors(DistanceSens *sensors);
/* Function to initialize telnet by opening up the port, requires WiFi to be active */
void initTelnet(void);

/* Function to start the telnet loop, requires telnet to be initialized. 
   Modules are optional but will cause exceptions if not set but used.   */
void startTelnet(void);


#endif //MyTelnet.h