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

// TODO: implement
const Container &ContainerStorage::removeContainer(std::string containerId) {
    return staticContainer;
}


// Getters and setters
const std::vector<Container> &ContainerStorage::getContainers() const {
    return containers;
}


// Printer
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
