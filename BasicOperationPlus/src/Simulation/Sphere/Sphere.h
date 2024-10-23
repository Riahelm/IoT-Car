#ifndef SPHERE_H
#define SPHERE_H

#include "Arduino.h"
#include <tuple>

class Sphere{
    public:
        Sphere(uint8_t radius, uint16_t xcoord, uint16_t ycoord);
        ~Sphere();
        void setCoords(uint16_t x, uint16_t y);
        std::tuple<uint16_t, uint16_t> getCoords(void);
        double getDistance(std::tuple<uint16_t, uint16_t> coords);
        double getDistance(uint16_t xcoord, uint16_t ycoord);
        uint8_t getRadius(void);
        bool isTouching(Sphere otherSphere);
        bool isInside(Sphere otherSphere);
        bool isOutside(Sphere otherSphere);

    private:
        uint8_t radius;
        uint16_t xcoord;
        uint16_t ycoord;
};

#endif