//
// Created by Orr on 4/9/2020.
//

#include "ContainerStorage.h"

//TODO: just to have something to return, can't return null reference
static Container tmpContainer (100, PortId("id"), "id");

//TODO: Return all containers for specified port
// The garage of a port
std::vector<Container> ContainerStorage::getContainersForDestination(const PortId &destId) {
    return std::vector<Container>();
}

void ContainerStorage::addContainer(const Container &container) {
    containers.push_back(container);
}

//TODO: Remove container
const Container &ContainerStorage::removeContainer(std::string containerId) {
    return tmpContainer;
}
