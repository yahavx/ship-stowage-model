//
// Created by yahav on 05/04/2020.
//

#include "Container.h"

Container::Container(int weight, const std::string &destPort, const std::string &id) : weight(weight), destPort(destPort),
                                                                             id(id) {

}

int Container::getWeight() const {
    return this->weight;
}

const std::string &Container::getDestPort() const {
    return this->destPort;
}

const std::string &Container::getId() const {
    return this->id;
}

void Container::setWeight(int weight) {
    this->weight = weight;
}

void Container::setDestPort(const std::string &destPort) {
    this->destPort = destPort;
}

void Container::setId(const std::string &id) {
    this->id = id;
}

std::ostream &operator<<(std::ostream &strm, const Container &cont) {
    return strm << "Container(weight=" << cont.getWeight() << ", destPort=" << cont.getDestPort()
                << ", id=" << cont.getId() << ")";
}