//
// Created by yahav on 05/04/2020.
//

#include "Container.h"

#include <utility>

Container::Container(int weight, const Port &destPort, std::string id) : weight(weight), destPort(destPort),
                                                                             id(std::move(id)) {

}

int Container::getWeight() const {
    return this->weight;
}

const Port &Container::getDestPort() const {
    return this->destPort;
}

const std::string &Container::getId() const {
    return this->id;
}

void Container::setWeight(int weight) {
    this->weight = weight;
}

void Container::setDestPort(const Port &destPort) {
    this->destPort = destPort;
}

void Container::setId(const std::string &id) {
    this->id = id;
}

std::ostream &operator<<(std::ostream &strm, const Container &cont) {
    return strm << "Container(weight=" << cont.getWeight() << ", destPort=" << cont.getDestPort()
                << ", id=" << cont.getId() << ")";
}