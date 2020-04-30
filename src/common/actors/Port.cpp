//
// Created by yahav on 06/04/2020.
//

#include "Port.h"
#include "../data_objects/PortId.h"
#include "../utils/UtilFunctions.h"

// region Constructors

Port::Port() : id("") {}

Port::Port(const PortId &id) : id(id) {}

Port::Port(const PortId &id, const ContainerStorage &storage) : id(id), storage(storage) {}
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

OptionalContainer Port::removeContainer(const std::string &containerId) {
    return storage.removeContainer(containerId);
}

bool Port::isIdInIsoFormat(const std::string &id) {
    if (id.length() != 11){
        return false;
    }

    if (!isEnglishWord(id.substr(0, 4)) || !isInteger(id.substr(4, 7))){
        return false;
    }

    return true;
}
// endregion

// region Getters and setters

const PortId &Port::getId() const {
    return id;
}

void Port::setId(const PortId &id) {
    Port::id = id;  // TODO: convert to upper case here and in the constructors (can user toUpper of UtilFunctions)
}

const ContainerStorage &Port::getStorage() const {
    return storage;
}

void Port::setStorage(const ContainerStorage &storage) {
    Port::storage = storage;
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