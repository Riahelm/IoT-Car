#include "StringParser.h"

uint32_t* getParam(String *);
uint8_t*  getFunction(String *);

std::tuple <uint8_t*, uint32_t*> extractFunc(String str){
    std::tuple <uint8_t*, uint32_t*> res = std::make_tuple(nullptr, nullptr);
    String myStr = str;
    myStr.trim();
    myStr.toLowerCase();

    std::get<0>(res) = getFunction(&myStr);

    std::get<1>(res) = getParam(&myStr);
    
    return res;
}

uint8_t * getFunction(String * str){
    String myStr = *str;
    uint8_t *res = (uint8_t *) malloc (sizeof(uint8_t));
    uint8_t state = 1;

    if(state == 1 && myStr.substring(0,2) == "go"){state = 2; myStr = myStr.substring(2);}
    else if (state == 1 && myStr.substring(0,4) == "turn"){state = 5; myStr = myStr.substring(4);}

    while((state == 2  || state == 5) && myStr[0] == ' '){myStr = myStr.substring(1);}

    if(state == 2 && myStr.substring(0,3) == "for"){state = 3; myStr = myStr.substring(3);  *res = 0;}
    else if(state == 2 && myStr.substring(0,4) == "back"){state = 3; myStr = myStr.substring(4); *res = 1;}

    if((state == 3) && myStr.substring(0,4) == "ward"){state = 4; myStr = myStr.substring(4);}
    if((state == 4) && myStr[0] == 's'){state = 6; myStr = myStr.substring(1);}

    if(state == 5 && myStr.substring(0,4) == "left") {state = 6; myStr = myStr.substring(4); *res = 2;}
    if(state == 5 && myStr.substring(0,5) == "right"){state = 6; myStr = myStr.substring(5); *res = 3;}

    if(((state == 4 || state == 6) && (myStr[0] != ' ' || !('0' < myStr[0] <= '9')))){res = nullptr;}
    
    if(state != 4 && state != 6){res = nullptr;}

    *str = myStr;

    return res;
}

uint32_t * getParam(String * str){
    String myStr = *str;
    uint32_t *res = (uint32_t *) malloc(sizeof(uint32_t));

    myStr.trim();
    
    *res = myStr.toInt();

    return *res == 0? nullptr : res;
}