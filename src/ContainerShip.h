//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CONTAINERSHIP_H
#define SHIP_STOWAGE_MODEL_CONTAINERSHIP_H

#include <vector>
#include "ContainerPlan.h"
#include "Container.h"
#include "PackingOperation.h"
#include "ShipRoute.h"
#include "WeightBalancer.h"

class ContainerShip {
    int X;  // length of the ship
    int Y;  // width of the ship
//    ShipPlan shipPlan;
    ShipRoute shipRoute;
//    WeightBalancer weightBalancer;

public:
//    const ShipPlan &getShipPlan();

    std::vector<PackingOperation> packing(std::vector<Container> containersToLoad);
};

class ShipFloor {
    int height;  // the floor index in the ship
//    FloorPlan plan;  // plan of the floor
};

#endif //SHIP_STOWAGE_MODEL_CONTAINERSHIP_H