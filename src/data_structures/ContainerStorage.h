//
// Created by Orr on 4/9/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CONTAINERSTORAGE_H
#define SHIP_STOWAGE_MODEL_CONTAINERSTORAGE_H

#include <vector>
#include <ostream>
#include "../data_objects/Container.h"
#include "../utils/Definitions.h"


static Container staticContainer("id", 100, PortId("id")); // TODO: remove this

class ContainerStorage {
    Containers containers;

public:

    // region Constructors

    ContainerStorage();

    ContainerStorage(const Containers &containers);
    // endregion

    // region Functions

    /**
     * Returns all containers in this cargo that are targeted to the given port
     * @param portId - target port
     */
    Containers getContainersForDestination(const PortId &destId);

    /**
     * Add container to this storage
     * @param container - container to add
     */
    void addContainer(const Container &container);

    /**
     * Add all given container to this storage
     * @param newContainers - newContainers to add
     */
    void addContainers(const Containers &newContainers);


    /**
     * Remove container from this storage
     * @param container - container to add
     */
    OptionalContainer removeContainer(const std::string& containerId);
    // endregion

    // region Getters and setters

    const Containers &getContainers() const;
    // endregion

    //region Printer

    friend std::ostream &operator<<(std::ostream &os, const ContainerStorage &storage);
    // endregion
};


#endif //SHIP_STOWAGE_MODEL_CONTAINERSTORAGE_H
