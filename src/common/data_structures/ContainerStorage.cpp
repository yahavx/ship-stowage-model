//
// Created by Orr on 4/9/2020.
//

#include "ContainerStorage.h"
#include "../utils/Definitions.h"

// region Constructors

ContainerStorage::ContainerStorage() {}


ContainerStorage::ContainerStorage(const Containers &containers) : containers(containers) {}
// endregion

// region Functions

Containers ContainerStorage::getContainersForDestination(const PortId &destId) {
    Containers result = Containers();
    for (const Container &container : containers)
        if (container.getDestPort() == destId) {
            result.push_back(container);
        }

    return result;
}

void ContainerStorage::addContainer(const Container &container) {
    containers.push_back(container);
}

void ContainerStorage::addContainers(const Containers &newContainers) {
    this->containers.insert(this->containers.end(), newContainers.begin(), newContainers.end());
}

OptionalContainer ContainerStorage::removeContainer(const std::string& containerId) {
    int foundIndex = -1;
    for (longUInt i = 0; i < containers.size(); i++) {
        Container container = containers[i];
        if (container.getId() == containerId) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex >= 0) {
        const Container container = containers[foundIndex];
        containers.erase(containers.begin() + foundIndex);
        return container;
    }

    return {};
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

    for (longUInt i = 0; i < storage.containers.size(); i++) {

        os << "\t\t" << i + 1 << ": " << storage.containers[i] << std::endl;
    }
    std::cout << "\t}" << std::endl;
    return os;
}
// endregion