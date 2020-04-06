//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SHIPPLAN_H
#define SHIP_STOWAGE_MODEL_SHIPPLAN_H


#include "FloorPlan.h"
#include <vector>


class ShipPlan {
    std::vector<FloorPlan> shipPlan;  // entry i is the floor plan of floor i
};


#endif //SHIP_STOWAGE_MODEL_SHIPPLAN_H
