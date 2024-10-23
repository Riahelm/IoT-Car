#include "Distance.h"

uint16_t getDistanceFromCoordinate(float x, float y, float x2, float y2){
    return sqrt(pow((x-x2, 2) + pow(y-y2, 2)))
}