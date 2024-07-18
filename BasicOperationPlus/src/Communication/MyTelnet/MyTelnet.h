#ifndef MYTELNET_H
#define MYTELNET_H

#include <tuple>
#include <ESPTelnet.h>
#include "../MyWiFi/MyWiFi.h"
#include "../../Actuators/Steppers/Steppers.h"
#include "../../Sensors/Distance/DistanceSens.h"
#include "../../Util/StringParser/StringParser.h"
#include "../../Util/Arrays/Arrays.h"

#define TELNET_PORT 23
#define THREAD_POLLING_INTERVAL 20
#define NUM_OF_COMMANDS 11

void initTelnet(void);

void startTelnet(void);

void setMotors(Steppers *steppers);

void setSensors(DistanceSens *sensors);

#endif //MyTelnet.h