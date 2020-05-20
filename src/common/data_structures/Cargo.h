//
// Created by Orr on 4/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CARGO_H
#define SHIP_STOWAGE_MODEL_CARGO_H

#include <ostream>
#include <unordered_map>
#include "../data_objects/ShipPlan.h"
#include "../data_objects/ContainerPosition.h"


/// Cargo is the stock of containers in the ship
class Cargo {
    ShipPlan shipPlan;
    std::vector<std::vector<std::vector<Container>>> containers;
    std::unordered_map<std::string, const Container&> containersMapping;  // hash of IDs, for quick checking and searching

public:

    // region Constructors

    Cargo();

    explicit Cargo(const ShipPlan &shipPlan);

    // endregion

    // region Booleans

private:
    bool validateXY(int x, int y) const;

public:
    /// Returns true if its possible to load the container in the position.
    bool canLoadContainerToPosition(int x, int y) const;

    /**
     * Returns true if this cargo is completely full
     */
    bool isFull();

    /**
     * Returns true if this cargo is completely full
     */
    bool isEmpty();

    /**
     * Returns true if the container with the given id exists
     */
    bool hasContainer(std::string containerId) const;

    // endregion

    // region Load/Unload


    /**
     * Removes and returns the top container in position (x,y)
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

    // endregion

    // region Others

    /**
     * If possible to load the container on top in this x,y position, returns the height it would be loaded to.
     * Otherwise returns -1.
     */
    int getAvailableFloorToLoadContainer(int x, int y) const;

    /**
     * Returns the current number of containers in position (x,y), or -1 if x or y are out of bounds.
     * @param x - row
     * @param y - col
     */
    int currentTopHeight(int x, int y) const;

    /**
     * Returns the top container in position (x,y)
     * @param x - row
     * @param y - col
     */
    OptionalContainer getTopContainer(int x, int y) const;

    /**
     * Returns positions for all containers in this cargo that are targeted to the given port
     * @param portId - target port
     */
    std::vector<ContainerPosition> getContainersForPort(const PortId &portId);

    /**
     * Returns the container if exists, or null.
     */
    OptionalContainer getContainerById(std::string containerId);

    /**
     * Returns The number of empty positions to load containers to
     */
    int numberOfEmptyPositions();

    // endregion

    // region Printer

    friend std::ostream &operator<<(std::ostream &os, const Cargo &cargo);

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_CARGO_H
