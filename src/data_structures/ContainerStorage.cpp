//
// Created by Orr on 4/9/2020.
//

#include "ContainerStorage.h"
#include "../utils/Definitions.h"

// region Functions

//TODO: Return all containers for specified port
// The garage of a port
Containers ContainerStorage::getContainersForDestination(const PortId &destId) {
    return Containers();
}

void ContainerStorage::addContainer(const Container &container) {
    containers.push_back(container);
}

void ContainerStorage::addContainers(const Containers &containers) {
    this->containers.insert(this->containers.end(), containers.begin(), containers.end());
}

// TODO: implement
const Container &ContainerStorage::removeContainer(std::string containerId) {
    return staticContainer;
}
// endregion

// region Getters and setters

const Containers &ContainerStorage::getContainers() const {
    return containers;
}
// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const ContainerStorage &storage) {
    if (storage.containers.size() == 0) {
        std::cout << "\t<EmptyContainerStorage>" << std::endl;
        return os;
    }

    std::cout << "\tContainerStorage {" << std::endl;

    for (int i = 0; i < storage.containers.size(); i++) {

        os << "\t\t" << i + 1 << ": " << storage.containers[i] << std::endl;
    }
    std::cout << "\t}" << std::endl;
    return os;
}
// endregion