//
// Created by yahav on 05/04/2020.
//

#include "Container.h"

#include <utility>


// Constructors
Container::Container(const std::string &id, int weight, const PortId &destPort) : id(id), weight(weight),
                                                                                  destPort(destPort) {}


// Getters and setters
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


// Printer
std::ostream &operator<<(std::ostream &os, const Container &container) {
    os << "Container(weight: " << container.weight << ", destPort: " << container.destPort << ", id: " << container.id
       << ")";
    return os;
}

bool Container::operator==(const Container &rhs) const {
    return id == rhs.id;
}

bool Container::operator!=(const Container &rhs) const {
    return !(rhs == *this);
}

