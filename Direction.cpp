#include "Directions.h"

int intFromDir(Directions dir){
    return dir == Directions::REVERSE? 0 : 1;
}

Directions dirFromInt(int dir){
    return dir == 0? Directions::REVERSE : Directions::FORWARD;
};