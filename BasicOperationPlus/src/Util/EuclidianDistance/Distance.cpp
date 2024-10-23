#include "Distance.h"

double getDistanceFromCoordinate(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2){
    return sqrt(pow(x-x2, 2) + pow(y-y2, 2));
};