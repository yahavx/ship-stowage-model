//
// Created by Orr on 4/9/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CONTAINERSTORAGE_H
#define SHIP_STOWAGE_MODEL_CONTAINERSTORAGE_H

#include <vector>
#include <ostream>
#include "../data_objects/Container.h"
#include "../utils/Definitions.h"


class ContainerStorage {
    Containers containers;
    StringVector badContainers;  // ids of bad containers

public:

    // region Constructors

    ContainerStorage();

    ContainerStorage(const Containers &containers);

    // endregion

    // region Getters and setters

    const Containers & getContainers() const;

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

    /// Add a container to the list of bad ids.
    void addBadContainer(const std::string &id);

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

    /// Removes from port all ids that are in containersIds. If an id doesn't exist, it is ignored.
    Containers removeContainers(const StringVector &containersIds);

    /**
     * Returns a container from this storage, by his id.
     * @return the first appearance on port if exists, otherwise null.
     */
    OptionalContainer getContainer(const std::string& containerId);

    /// Removes the last appearance of a container from the port.
    void removeContainerFromEnd(const std::string& containerId);

    bool hasContainer(const std::string& containerId);

    bool isEmpty() const;

    // endregion

    //region Printer

    friend std::ostream &operator<<(std::ostream &os, const ContainerStorage &storage);

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_CONTAINERSTORAGE_H
