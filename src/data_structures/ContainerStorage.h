//
// Created by Orr on 4/9/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CONTAINERSTORAGE_H
#define SHIP_STOWAGE_MODEL_CONTAINERSTORAGE_H


#include <vector>
#include "../data_objects/Container.h"

class ContainerStorage {
    std::vector<Container> containers;

public:
    std::vector<Container> getContainersForDestination(const PortId &destId);

    void addContainer(const Container &container);

    Container &removeContainer(std::string containerId);
};


#endif //SHIP_STOWAGE_MODEL_CONTAINERSTORAGE_H
