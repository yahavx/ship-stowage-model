//
// Created by yahav on 06/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_PORT_H
#define SHIP_STOWAGE_MODEL_PORT_H

#include <iostream>
#include <stdio.h>
#include "../data_structures/ContainerStorage.h"
#include "../data_objects/PortId.h"
#include "../utils/Definitions.h"

class ShipRoute;

class Port {
    PortId id;
    ContainerStorage storage;

public:
    // region Constructors

    Port();

    explicit Port(const PortId &id);

    Port(const PortId &id, const ContainerStorage &storage);

    // endregion

    //region Getters and setters

    const PortId &getId() const;

    void setId(const PortId &id);

    const ContainerStorage &getStorage() const;

    void setStorage(const ContainerStorage &storage);

    // endregion

    // region Functions

    Containers getContainersForDestination(const PortId &destId);

    void addContainer(const Container &container);

    void addContainers(const Containers &containers);

    OptionalContainer removeContainer(const std::string &containerId);

    /**
     * Removes all containers that can't be loaded from the port (duplicate ID, not on route, etc).
     *
     * @param errors all errors found, in each container, will be added to this
     * @return list of id's, one per each removed container.
     */
    StringVector removeBadContainers(const ShipRoute& route, Errors &errors = e_garbageCollector);

    bool isDuplicateIdOnPort(const std::string &containerId);

    /// Returns the number of containers on port with the following id.
    int containersWithSameIdOnPort(const std::string &containerId);

    /// Checks if the port storage contains a container with the following id.
    bool hasContainer(const std::string &containerId);

    // endregion

    // region Printer

    friend std::ostream &operator<<(std::ostream &os, const Port &port);

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_PORT_H
