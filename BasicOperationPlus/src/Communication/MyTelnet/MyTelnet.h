#ifndef MYTELNET_H
#define MYTELNET_H

#include "../MyWiFi/MyWiFi.h"
#include <ESPTelnet.h>
#include "../Steppers/Steppers.h"
#include "../Sensors/Distance/DistanceSens.h"
#include <list>

#define TELNET_PORT 23
#define THREAD_POLLING_INTERVAL 20

void initTelnet(void);

void startTelnet(void);

void setMotors(Steppers *steppers);

void setSensors(DistanceSens *sensors);

#endif //MyTelnet.h