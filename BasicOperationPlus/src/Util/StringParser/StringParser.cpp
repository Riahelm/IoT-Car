#include "StringParser.h"

using namespace std;

tuple<String, String> splitFunc(String str)
{   
    uint8_t i = 0;
    if(str.length() > 0){
        while(str[i] != ' ' ){i++;}
    }
    
    return make_tuple(str.substring(0, i), str.substring(i));
}