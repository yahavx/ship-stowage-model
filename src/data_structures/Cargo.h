//
// Created by Orr on 4/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CARGO_H
#define SHIP_STOWAGE_MODEL_CARGO_H

#include "../data_objects/ShipPlan.h"
#include "../data_objects/ContainerPosition.h"


/// Cargo is the stock of containers in the ship
class Cargo {
    std::vector<std::vector<std::vector<Container>>> containers;

    ShipPlan &shipPlan;

public:
    /**
     * If possible to load the container on top in this x,y position returns the height it would be loaded to.
     * else returns negative integer
     */
    int canLoadContainerOnTop(int x, int y, const Container &container) const;

    /**
     * Returns the current number of containers in position (x,y), or -1 if x or y are out of bounds.
     * @param x - row
     * @param y - col
     */
    int currentNumContainers(int x, int y) const;

    /**
     * Returns the top container in position (x,y)
     * //TODO: Switch to optional
     * @param x - row
     * @param y - col
     */
    OptionalContainer getTopContainer(int x, int y) const;

    /**
     * Returns positions for all containers in this cargo that are targeted to the given port
     * @param portId - target port
     */
    std::vector<ContainerPosition> getContainersForPort(const PortId &portId) const;

    /**
     * Removes and returns the top container in position (x,y)
     * //TODO: Switch to optional
     * @param x - row
     * @param y - col
     */
    OptionalContainer removeTopContainer(int x, int y);

    /**
     * Loads container to position (x,y) of possible, and returns the height it was loaded to
     * @param x - row
     * @param y - col
     * @param container - container to load
     * @return - if operation succeeded returns the height te container was loaded to, else returns negative integer
     */
    int loadContainerOnTop(int x, int y, const Container &container);
};


#endif //SHIP_STOWAGE_MODEL_CARGO_H
