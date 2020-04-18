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
    // region Functions

    Containers getContainersForDestination(const PortId &destId);

    void addContainer(const Container &container);

    void addContainers(const Containers&containers);

    const Container &removeContainer(std::string containerId);
    // endregion

    // region Getters and setters

    const Containers &getContainers() const;
    // endregion

    //region Printer

    friend std::ostream &operator<<(std::ostream &os, const ContainerStorage &storage);
    // endregion
};


#endif //SHIP_STOWAGE_MODEL_CONTAINERSTORAGE_H
