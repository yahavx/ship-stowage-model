//
// Created by Orr on 4/9/2020.
//

#include "ContainerStorage.h"

//TODO: Return all containers for specified port
// The garage of a port
std::vector<Container> ContainerStorage::getContainersForDestination(const PortId &destId) {
    return std::vector<Container>();
}

void ContainerStorage::addContainer(const Container &container) {
    containers.push_back(container);
}

void ContainerStorage::addContainers(const std::vector<Container> &containers) {
    this->containers.insert(this->containers.end(), containers.begin(), containers.end());
}

//TODO: Remove container
const Container &ContainerStorage::removeContainer(std::string containerId) {
    return Container("id", 100, PortId("id"));
}


// Getters and setters
const std::vector<Container> &ContainerStorage::getContainers() const {
    return containers;
}


// Printer
std::ostream &operator<<(std::ostream &os, const ContainerStorage &storage) {
    if (storage.containers.size() == 0) {
        std::cout << "<EmptyContainerStorage>" << std::endl;
        return os;
    }

    for (int i = 0; i < storage.containers.size(); i++) {
        os << " " << i + 1 << ": " << storage.containers[i] << std::endl;
    }

    return os;
}
