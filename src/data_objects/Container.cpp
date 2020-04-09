//
// Created by yahav on 05/04/2020.
//

#include "Container.h"

#include <utility>

int Container::getWeight() const {
    return weight;
}

void Container::setWeight(int weight) {
    Container::weight = weight;
}

const PortId &Container::getDestPort() const {
    return destPort;
}

void Container::setDestPort(const PortId &destPort) {
    Container::destPort = destPort;
}

const std::string &Container::getId() const {
    return id;
}

void Container::setId(const std::string &id) {
    Container::id = id;
}

Container::Container(int weight, const PortId &destPort, const std::string &id) : weight(weight), destPort(destPort),
                                                                                id(id) {}

std::ostream &operator<<(std::ostream &os, const Container &container) {
    os << "weight: " << container.weight << " destPort: " << container.destPort << " id: " << container.id;
    return os;
}