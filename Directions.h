#ifndef DIRECTIONS_H
#define DIRECTIONS_H

enum class Directions{
    REVERSE,
    FORWARD,
};

int intFromDir(Directions dir);

Directions dirFromInt(int dir);

#endif // DIRECTIONS_H

