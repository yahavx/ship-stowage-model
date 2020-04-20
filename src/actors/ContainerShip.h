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

    void markCurrentVisitDone();

    const ShipPlan &getShipPlan() const;

    void setShipPlan(const ShipPlan &shipPlan);

    ShipRoute &getShipRoute();

    void setShipRoute(ShipRoute &shipRoute);

    Cargo &getCargo();  // Not const

    void setCargo(const Cargo &cargo);

    WeightBalanceCalculator &getBalanceCalculator() const;

    void setBalanceCalculator(WeightBalanceCalculator &balanceCalculator);
    // endregion

    // region Functions

    /**
     * Receives the next id of port to dock into, and list of containers that needs to be loaded.
     * @return list of operations: first unload all containers for this port, and than load all the containers that needs to be loaded.
    */
    std::vector<PackingOperation> dock(Port &port, const Containers &containersToLoad);


private:
    /**
     * returns legal PackingOperation for loading the container to an arbitrary position.
     *
     * @param container - container to load
     * @return legal PackingOperation or NULL if none was found
     */
    std::vector<PackingOperation> loadContainerToArbitraryPosition(Port &port, const Container &container);

    /**
    * returns legal series of PackingOperation's for unloading container.
    *
    * @param container - container to load
    * @return legal series of PackingOperation's or NULL if none was found
    */
    std::vector<PackingOperation> unloadContainer(Port &port, const ContainerPosition &container);
    // endregion
};

#endif //SHIP_STOWAGE_MODEL_CONTAINERSHIP_H