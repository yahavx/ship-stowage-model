//
// Created by yahav on 06/04/2020.
//

#include "Port.h"
#include "../data_objects/PortId.h"
#include "../utils/UtilFunctions.h"

// Constructors
Port::Port(const PortId &id) : id(id) {}

Port::Port(const PortId &id, const ContainerStorage &storage) : id(id), storage(storage) {}


Containers Port::getContainersForDestination(const PortId &destId) {
    return storage.getContainersForDestination(destId);
}


// Functions
void Port::addContainer(const Container &container) {
    return storage.addContainer(container);
}

void Port::addContainers(const Containers &containers) {
    return storage.addContainers(containers);
}

const Container &Port::removeContainer(std::string containerId) {
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


// Getters and setters
const PortId &Port::getId() const {
    return id;
}

void Port::setId(const PortId &id) {
    Port::id = id;
}


// Printer
std::ostream &operator<<(std::ostream &os, const Port &port) {
    os << "Port {" << std::endl;
    os << "\t" << port.id << std::endl;
    os << port.storage;
    os << "}" << std::endl;
    return os;
}

