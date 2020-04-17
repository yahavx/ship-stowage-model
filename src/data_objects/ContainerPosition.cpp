//
// Created by Orr on 4/16/2020.
//

#include "ContainerPosition.h"

ContainerPosition::ContainerPosition(Container &container, const POS &position) : container(
        container), position(position) {}

Container &ContainerPosition::getContainer() const {
    return container;
}

void ContainerPosition::setContainer(Container &container) {
    ContainerPosition::container = container;
}

const POS &ContainerPosition::getPosition() const {
    return position;
}

void ContainerPosition::setPosition(const POS &position) {
    ContainerPosition::position = position;
}

int ContainerPosition::x() const {
    return std::get<0>(position);
}

int ContainerPosition::y() const {
    return std::get<1>(position);
}

int ContainerPosition::z() const {
    return std::get<2>(position);
}
