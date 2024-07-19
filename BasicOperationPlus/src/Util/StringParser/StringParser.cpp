#include "StringParser.h"

using namespace std;

tuple<String, String> splitFunc(String str, char delimiter)
{   
    /* Empty tuple, required to define in order to deal with specific strings */
    tuple<String, String> res("", "");
    uint8_t i = 0;
    if(str.length() > 0){
        /* Checks character by character until delimiter is found or whole string has been inspected */
        while(str[i] != delimiter && str.length() - i > 0){i++;}

        get<0>(res) = str.substring(0, i);
        get<1>(res) = str.substring(i);
    }
    
    return res;
}