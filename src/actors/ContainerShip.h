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
    Cargo cargo;
    WeightBalanceCalculator *balanceCalculator;

public:
    // region Constructors

    ContainerShip();

    ContainerShip(const ShipPlan &shipPlan, const ShipRoute &shipRoute,
                  WeightBalanceCalculator &balanceCalculator);

    ContainerShip(const ShipPlan &shipPlan, const ShipRoute &shipRoute);
    // endregion

    // region Getters and setters

    const ShipPlan &getShipPlan() const;

    void setShipPlan(const ShipPlan &shipPlan);

    const ShipRoute &getShipRoute() const;

    void setShipRoute(const ShipRoute &shipRoute);

    Cargo &getCargo();  // Not const

    void setCargo(const Cargo &cargo);

    const WeightBalanceCalculator &getBalanceCalculator() const;

    void setBalanceCalculator(WeightBalanceCalculator &balanceCalculator);
    // endregion

    // region Functions

    /**
     * Receives the next id of port to dock into, and list of containers that needs to be loaded.
     * @return list of operations: first unload all containers for this port, and than load all the containers that needs to be loaded.
    */
    std::vector<PackingOperation> dock(const PortId &portId, const Containers &containersToLoad);


private:
    /**
     * returns legal PackingOperation for loading the container to an arbitrary position.
     * If there was no available legal position found returns NULL TODO: check how to return null
     *
     * @param container - container to load
     * @return legal PackingOperation or NULL if none was found
     */
    std::vector<PackingOperation> loadContainerToArbitraryPosition(const Container &container);

    /**
    * returns legal series of PackingOperation's for unloading container.
    * If there was no available series legal of PackingOperation's found returns NULL TODO: check how to return null
    *
    * @param container - container to load
    * @return legal series of PackingOperation's or NULL if none was found
    */
    std::vector<PackingOperation> unloadContainer(const ContainerPosition &container);
    // endregion
};

#endif //SHIP_STOWAGE_MODEL_CONTAINERSHIP_H