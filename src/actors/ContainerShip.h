//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CONTAINERSHIP_H
#define SHIP_STOWAGE_MODEL_CONTAINERSHIP_H

#include <vector>
#include "../data_objects/ShipPlan.h"
#include "../data_objects/Container.h"
#include "../data_objects/PackingOperation.h"
#include "../data_objects/ShipRoute.h"
#include "../algorithms/WeightBalanceCalculator.h"

//class ShipPlan;
//class ShipRoute;
//class WeightBalanceCalculator;

class ContainerShip {
    ShipPlan shipPlan;
    ShipRoute shipRoute;
    WeightBalanceCalculator balanceCalculator;

public:
    std::vector<PackingOperation> dock(const PortId &portId, const std::vector<Container> &containersToLoad);
};

#endif //SHIP_STOWAGE_MODEL_CONTAINERSHIP_H