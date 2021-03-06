//
// Created by Orr on 4/9/2020.
//

#include "ContainerStorage.h"
#include "../utils/Definitions.h"

// region Constructors

ContainerStorage::ContainerStorage() {}


ContainerStorage::ContainerStorage(const Containers &containers) : containers(containers) {}
// endregion

// region Getters and setters

const Containers & ContainerStorage::getContainers() const {
    return containers;
}

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

void ContainerStorage::addBadContainer(const std::string &id) {
    badContainers.push_back(id);
}

void ContainerStorage::addContainers(const Containers &newContainers) {
    this->containers.insert(this->containers.end(), newContainers.begin(), newContainers.end());
}

OptionalContainer ContainerStorage::removeContainer(const std::string &containerId) {
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

Containers ContainerStorage::removeContainers(const StringVector &containersIds) {
    Containers removedContainers;

    for (auto &badContainerId : containersIds) {
        auto cont = this->removeContainer(badContainerId);
        if (cont.has_value()) {
            removedContainers.push_back(*cont);
        }
    }

    return removedContainers;
}

OptionalContainer ContainerStorage::getContainer(const std::string& containerId) {
    for (auto& container : containers) {
        if (container.getId() == containerId) {
            return container;
        }
    }

    return std::nullopt;
}

void ContainerStorage::removeContainerFromEnd(const std::string &containerId) {
    for (int i = containers.size() - 1; i >= 0; i--) {
        if (containers[i].getId() == containerId) {
            containers.erase(containers.begin() + i);
            break;
        }
    }
}

bool ContainerStorage::hasContainer(const std::string &containerId) {
    for (auto &container : containers) {
        if (container.getId() == containerId) {
            return true;
        }
    }
    return false;
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

bool ContainerStorage::isEmpty() const {
    return containers.empty();
}
// endregion