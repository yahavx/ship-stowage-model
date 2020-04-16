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
#include "../data_structures/Cargo.h"

class ContainerShip {
    ShipPlan shipPlan;
    Cargo cargo;
    ShipRoute shipRoute;
    WeightBalanceCalculator balanceCalculator;

public:

    std::vector<PackingOperation> dock(const PortId &portId, const std::vector<Container> &containersToLoad);

    const ShipPlan &getShipPlan() const;

    void setShipPlan(const ShipPlan &shipPlan);

    const Cargo &getCargo() const;

    void setCargo(const Cargo &cargo);

    const ShipRoute &getShipRoute() const;

    void setShipRoute(const ShipRoute &shipRoute);

    const WeightBalanceCalculator &getBalanceCalculator() const;

    void setBalanceCalculator(const WeightBalanceCalculator &balanceCalculator);
};

#endif //SHIP_STOWAGE_MODEL_CONTAINERSHIP_H