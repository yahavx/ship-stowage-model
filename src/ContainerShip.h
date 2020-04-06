//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CONTAINERSHIP_H
#define SHIP_STOWAGE_MODEL_CONTAINERSHIP_H

#include <vector>
#include "ShipPlan.h"
#include "Container.h"
#include "PackingOperation.h"
#include "ShipRoute.h"
#include "IWeightBalancer.h"

class ContainerShip {
    int height;  // height of the shap
    int width;  // width of the ship
    ShipPlan shipPlan;
    ShipRoute shipRoute;
    IWeightBalancer weightBalancer;

public:
    const ShipPlan &getShipPlan();

    vector<PackingOperation> packing(std::vector<Container> containersToLoad);
};

#endif //SHIP_STOWAGE_MODEL_CONTAINERSHIP_H
