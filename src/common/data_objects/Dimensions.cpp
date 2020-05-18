//
// Created by yahav on 13/05/2020.
//

#include <ostream>
#include "Dimensions.h"

// region Constructor

Dimensions::Dimensions(int x, int y, int floor) : Vector3(x,y,floor) {}

// endregion

// region Getters and setters

int Dimensions::X() const {
    return x;
}

int Dimensions::Y() const {
    return y;
}

int Dimensions::Z() const {
    return z;
}

// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const Dimensions &dimensions) {
    os << "(X: " << dimensions.x << ", Y: " << dimensions.y << "), Floors: " << dimensions.z;
    return os;
}

// endregion