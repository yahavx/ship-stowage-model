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
    ShipRoute shipRoute;
    WeightBalanceCalculator balanceCalculator;

public:

    std::vector<PackingOperation> dock(const PortId &portId, const std::vector<Container> &containersToLoad);

    const ShipPlan &getShipPlan() const;

    void setShipPlan(const ShipPlan &shipPlan);

    Cargo &getCargo();

    void setCargo(const Cargo &cargo);

    const ShipRoute &getShipRoute() const;

    void setShipRoute(const ShipRoute &shipRoute);

    const WeightBalanceCalculator &getBalanceCalculator() const;

    void setBalanceCalculator(const WeightBalanceCalculator &balanceCalculator);

private:
    /**
     * returns legal PackingOperation for loading the container to an arbitrary position.
     * If there was no available legal position found returns NULL TODO: check how to return null
     *
     * @param container - container to load
     * @return legal PackingOperation or NULL if none was found
     */
    std::vector<PackingOperation> loadContainerToaArbitraryPosition(const Container &container);

    /**
    * returns series legal of PackingOperation's for unloading container.
    * If there was no available series legal of PackingOperation's found returns NULL TODO: check how to return null
    *
    * @param container - container to load
    * @return legal series of PackingOperation's or NULL if none was found
    */
    std::vector<PackingOperation> unloadContainer(const ContainerPosition &container);
};

#endif //SHIP_STOWAGE_MODEL_CONTAINERSHIP_H