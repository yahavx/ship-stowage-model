//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SHIPFLOOR_H
#define SHIP_STOWAGE_MODEL_SHIPFLOOR_H


#include "ContainerPlan.h"

class ShipFloor {
    int height;  // the floor index in the ship

    ShipPlan plan;  // plan of the floor
};

#endif //SHIP_STOWAGE_MODEL_SHIPFLOOR_H
