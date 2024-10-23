#include "Sphere.h"
#include "../../Util/EuclidianDistance/Distance.h"

Sphere::Sphere(uint8_t radius, uint16_t xcoord, uint16_t ycoord){
    this->radius = radius;
    this->xcoord = xcoord;
    this->ycoord = ycoord;

};

Sphere::~Sphere(){};

void Sphere::setCoords(uint16_t x, uint16_t y){
    this->xcoord = x;
    this->ycoord = y;
};

std::tuple<uint16_t, uint16_t> Sphere::getCoords(void){
    return std::make_tuple(this->xcoord, this->ycoord);
};

double Sphere::getDistance(uint16_t x, uint16_t y){
    return getDistanceFromCoordinate(this->xcoord, this->ycoord, x, y);
};

double Sphere::getDistance(std::tuple<uint16_t, uint16_t> coords){
    return this->getDistance(std::get<0>(coords), std::get<1>(coords));
};

uint8_t Sphere::getRadius(){
    return this->radius;
};

bool Sphere::isTouching(Sphere otherSphere){
    return this->getDistance(otherSphere.getCoords()) <= this->radius + otherSphere.getRadius();
};

bool Sphere::isInside(Sphere otherSphere){
    return this->getDistance(otherSphere.getCoords()) <= otherSphere.getRadius();
};

bool Sphere::isOutside(Sphere otherSphere){
    return this->getDistance(otherSphere.getCoords()) >= otherSphere.getRadius();
};
