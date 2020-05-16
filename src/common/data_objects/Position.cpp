//
// Created by Orr on 4/17/2020.
//

#include "Position.h"

// region Constructor

Position::Position(int x, int y, int floor) : Vector3(x,y,floor) {}

// endregion

// region Getters and setters

int Position::X() const {
    return x;
}

int Position::Y() const {
    return y;
}

int Position::floor() const {
    return z;
}

// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const Position &position) {
    os << "(" << position.x << ", " << position.y << ", " << position.z << ")";
    return os;
}

// endregion