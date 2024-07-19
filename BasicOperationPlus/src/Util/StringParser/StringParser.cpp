#include "StringParser.h"

using namespace std;

tuple<String, String> splitFunc(String str)
{   
    tuple<String, String> res("", "");
    uint8_t i = 0;
    if(str.length() > 0){
        while(str[i] != ' ' && str.length() - i > 0){i++;}

        get<0>(res) = str.substring(0, i);
        get<1>(res) = str.substring(i);
    }
    
    return res;
}