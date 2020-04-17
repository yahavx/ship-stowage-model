//
// Created by Orr on 4/9/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CONTAINERSTORAGE_H
#define SHIP_STOWAGE_MODEL_CONTAINERSTORAGE_H

#include <vector>
#include <ostream>
#include "../data_objects/Container.h"


class ContainerStorage {
    std::vector<Container> containers;

public:
    // Functions
    std::vector<Container> getContainersForDestination(const PortId &destId);

    void addContainer(const Container &container);

    void addContainers(const std::vector<Container> &containers);

    const Container &removeContainer(std::string containerId);


    // Getters and setters
    const std::vector<Container> &getContainers() const;

    friend std::ostream &operator<<(std::ostream &os, const ContainerStorage &storage);
};


#endif //SHIP_STOWAGE_MODEL_CONTAINERSTORAGE_H
