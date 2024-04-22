#include "Direction.h"

int intFromDir(Direction dir){
    return dir == Direction::REVERSE? 0 : 1;
}

Direction dirFromInt(int dir){
    return dir == 0? Direction::REVERSE : Direction::FORWARD;
};