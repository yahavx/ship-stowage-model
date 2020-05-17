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
#include "NaiveWeightBalancer.h"
#include "../data_structures/Cargo.h"
#include "../data_objects/Operations.h"
#include "../strategies/LoadContainerStrategy.h"

class LoadContainerStrategy;

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

    ShipRoute &getShipRoute();

    void setShipRoute(ShipRoute &shipRoute);

    Cargo &getCargo();  // Not const

//    void setCargo(const Cargo &cargo);  // May cause problems

    WeightBalanceCalculator &getBalanceCalculator() const;

    void setBalanceCalculator(WeightBalanceCalculator &balanceCalculator);

    // endregion

    // region Functions

    void advanceToNextPort();

    /// Returns the current (first) port in the route. If there is no port, it is an error.
    PortId getCurrentPortId();


    /**
     * returns legal PackingOperation for loading the container to an arbitrary position,
     * using the given strategy
     *
     * @param container - container to load
     * @return legal PackingOperation or NULL if none was found
     */
    Operations loadContainer(LoadContainerStrategy* strategy, Port &port, const Container &container, Errors &errors);

    /**
    * returns legal series of PackingOperation's for unloading container.
    *
    * @param container - container to load
    * @return legal series of PackingOperation's or NULL if none was found
    */
    Operations unloadContainer(Port &port, const ContainerPosition &container);


    /**
     * Returns all ids of containers that can't be loaded to that ship (ID already on ship, not on route, etc).
     *
     * @param errors all errors found, in each container, will be added to this
     * @return list of id's, one per each illegal container.
     */
    StringVector filterContainers(const Containers &containersToLoad, Errors &errors);

    /// Returns the container with the matching id. If not exists, return null.
    OptionalContainer getContainerById(const std::string &id);

    bool isContainerOnShip(const std::string &containerId);

    // endregion
};

#endif //SHIP_STOWAGE_MODEL_CONTAINERSHIP_H