
#ifndef SIMULATION_MAP.H
#define SIMULATION_MAP.H

#include "Arduino.h"
#include "../Util/Arrays/Arrays.h"

class SimulationMap{
    public:
        SimulationMap(const char (&map)[10][10], uint8_t scale);
        SimulationMap(uint16_t size, uint8_t scale, uint16_t posX, uint16_t posY);
        
        char grid[10][10];
        uint8_t scale;
};

#endif //SimulationMap.h