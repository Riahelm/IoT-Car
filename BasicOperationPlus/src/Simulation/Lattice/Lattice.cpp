#include "Lattice.h"

Lattice::Lattice(uint16_t sizeY, uint16_t sizeX, uint16_t goalX, uint16_t goalY){
    this->xs = sizeX;
    this->ys = sizeY; 
    this->digitalMap = (bool **)calloc(sizeof(bool *), sizeY);

    for(int i = 0; i < sizeY; i++){
        digitalMap[i] = (bool *)calloc(sizeof(bool), sizeX);
        for(int j = 0; j < sizeX; j++){
            digitalMap[i][j] = false;
        }
    } /* THE DESTRUCTOR !!!*/

};

/*Lattice::start()
{
};*/
