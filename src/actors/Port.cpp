//
// Created by yahav on 06/04/2020.
//

#include "Port.h"
#include "../data_objects/PortId.h"

std::vector<Container> Port::getContainersForDestination(const PortId &destId) {
    return storage.getContainersForDestination(destId);
}

Port::Port(const PortId &id) : id(id) {}

const PortId &Port::getId() const {
    return id;
}

void Port::setId(const PortId &id) {
    Port::id = id;
}

void Port::addContainer(const Container &container) {
    return storage.addContainer(container);
}

const Container &Port::removeContainer(std::string containerId) {
    return storage.removeContainer(containerId);
}

std::ostream &operator<<(std::ostream &os, const Port &port) {
    os << "id: " << port.id;
    return os;
}
