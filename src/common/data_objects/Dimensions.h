//
// Created by yahav on 13/05/2020.
//

#ifndef SHIP_STOWAGE_MODEL_DIMENSIONS_H
#define SHIP_STOWAGE_MODEL_DIMENSIONS_H


#include "../utils/Vector3.h"

class Dimensions : public Vector3 {

public:

    // region Constructor

    Dimensions() = default;

    Dimensions(int X, int Y, int floors);

    // endregion

    // region Getters and setters

    int X() const;

    int Y() const;

    int Z() const;

    // endregion

    // region Printer

    friend std::ostream &operator<<(std::ostream &os, const Dimensions &dimensions);

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_DIMENSIONS_H
