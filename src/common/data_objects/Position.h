//
// Created by Orr on 4/17/2020.
//

#ifndef SHIP_STOWAGE_MODEL_POSITION_H
#define SHIP_STOWAGE_MODEL_POSITION_H


#include <ostream>
#include "../utils/Vector3.h"

/// Describes a position in the ship.
class Position : public Vector3 {

public:

    // region Constructor

    Position(int x, int y, int floor);

    // endregion

    // region Getters and setters

    int X() const;

    int Y() const;

    int Z() const;

    // endregion

    // region Printer

    friend std::ostream &operator<<(std::ostream &os, const Position &position);

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_POSITION_H
