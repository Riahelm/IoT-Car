#ifndef LATTICE_H
#define LATTICE_H

#include "Arduino.h"

class Lattice{
    public:
        Lattice(uint16_t sizeX, uint16_t sizeY, uint16_t goalX, uint16_t goalY);

        //start();
    
    private:
        
        uint16_t xs;
        uint16_t ys;
        bool **digitalMap;
};

#endif