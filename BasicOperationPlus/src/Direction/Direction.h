#ifndef DIRECTION_H
#define DIRECTION_H

enum class Direction{
    REVERSE,
    FORWARD,
};

int intFromDir(Direction dir);

Direction dirFromInt(int dir);

#endif // DIRECTION_H

