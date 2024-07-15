#ifndef MYTELNET_H
#define MYTELNET_H

#include "../MyWiFi/MyWiFi.h"
#include <ESPTelnet.h>
#include <list>

#define TELNET_PORT 23

ESPTelnet telnet;

class MyTelnet{
    public:
        MyTelnet(void);
        static ESPTelnet telnet;
        void startTelnet(void);
    private:
        void onTelnetConnect(String ip);
        void onTelnetConnectionAttempt(String ip);
        void onTelnetReconnect(String ip);
        void onTelnetDisconnect(String ip);
        void onTelnetInput(String msg);
}




#endif //MyTelnet.h