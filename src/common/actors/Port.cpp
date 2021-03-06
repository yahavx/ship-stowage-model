//
// Created by yahav on 06/04/2020.
//

#include <unordered_map>
#include <unordered_set>
#include "Port.h"
#include "../data_objects/PortId.h"
#include "../utils/UtilFunctions.h"
#include "../data_objects/ShipRoute.h"


// region Constructors

Port::Port() : id("") {}

Port::Port(const PortId &id) : id(id) {}

Port::Port(const PortId &id, const ContainerStorage &storage) : id(id), storage(storage) {}
// endregion

// region Getters and setters

const PortId &Port::getId() const {
    return id;
}

void Port::setId(const PortId &id) {
    Port::id = id;
}

const ContainerStorage &Port::getStorage() const {
    return storage;
}

void Port::setStorage(const ContainerStorage &storage) {
    Port::storage = storage;
}

void Port::setStorage(ContainerStorage &&storage) {
    Port::storage = storage;
}

// endregion

// region Functions

Containers Port::getContainersForDestination(const PortId &destId) {
    return storage.getContainersForDestination(destId);
}

void Port::addContainer(const Container &container) {
    return storage.addContainer(container);
}

void Port::addContainers(const Containers &containers) {
    return storage.addContainers(containers);
}

Containers Port::removeContainers(const StringVector &containersIds) {
    return storage.removeContainers(containersIds);
}

OptionalContainer Port::removeContainer(const std::string &containerId) {
    return storage.removeContainer(containerId);
}

StringVector Port::removeBadContainers(Errors &errors) {
    StringVector invalidContainersIds;
    std::unordered_set<std::string> containerIds;  // Collect all ids to detect duplicates

    for (auto &container : storage.getContainers()) {
        auto& contId = container.getId();

        if (containerIds.find(contId) == containerIds.end()) {
            containerIds.insert(contId);
        }
        else {  // Already seen this id
            errors.addError({ErrorFlag::ContainersAtPort_DuplicateID, contId});
            invalidContainersIds.push_back(contId);
            continue;
        }

        Error err = container.isContainerLegal();

        // Illegal containers
        if (!err.isSuccess()) {
            errors.addError(err);
            invalidContainersIds.push_back(contId);
            continue;
        }

        // Container destination is current port
        if (container.getDestPort() == id) {
            errors.addError({ContainersAtPort_ContainerDestinationIsCurrentPort, container.getId()});
            invalidContainersIds.push_back(contId);
            continue;
        }
    }

    for (auto& badContainerId : invalidContainersIds) {
        storage.removeContainerFromEnd(badContainerId);  // Remove from end to keep the first copy in case of duplicates
        storage.addBadContainer(badContainerId);  // add to bad list
    }

    return invalidContainersIds;
}

bool Port::isDuplicateIdOnPort(const std::string &containerId)  {
    return containersWithSameIdOnPort(containerId) > 1;
}

int Port::containersWithSameIdOnPort(const std::string &containerId) {
    int count = 0;

    for (auto& container : storage.getContainers()) {
        if (container.getId() == containerId) {
            count++;
        }
    }

    return count;
}


bool Port::hasContainer(const std::string &containerId) {
    return storage.hasContainer(containerId);
}

OptionalContainer Port::getContainer(const std::string& containerId) {
    return storage.getContainer(containerId);
}

// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const Port &port) {
    os << "Port {" << std::endl;
    os << "\t" << port.id << std::endl;
    os << port.storage;
    os << "}" << std::endl;
    return os;
}

// endregion