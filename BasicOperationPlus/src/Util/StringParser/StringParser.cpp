#include "StringParser.h"

bool valid(String str)
{   
    String myStr = str;
    myStr.trim();
    myStr.toLowerCase();
    bool res = false;
    uint8_t state = 1;
    uint8_t i = 0;

    if(state == 1 && myStr.substring(0,2) == "go"){state = 2; myStr = myStr.substring(2);}
    while(state == 2 && myStr[0] == ' '){myStr = myStr.substring(1);}
    if(state == 2 && myStr.substring(0,3) == "for"){state = 3; myStr = myStr.substring(3);}
    else if(state == 2 && myStr.substring(0,4) == "back"){state = 3; myStr = myStr.substring(4);}
    if(state == 3 && myStr.substring(0,4) == "ward"){state = 4; myStr = myStr.substring(4); res = true;}
    if(state == 4 && myStr[0] == 's'){state = 5; myStr = myStr.substring(1);}
    if(state == 5 && myStr.length() != 0){res = false;}
    
    return res;

}