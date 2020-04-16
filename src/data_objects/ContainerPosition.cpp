//
// Created by Orr on 4/16/2020.
//

#include "ContainerPosition.h"

ContainerPosition::ContainerPosition(Container &container, const std::tuple<int, int, int> &position) : container(
        container), position(position) {}

Container &ContainerPosition::getContainer() const {
    return container;
}

void ContainerPosition::setContainer(Container &container) {
    ContainerPosition::container = container;
}

const std::tuple<int, int, int> &ContainerPosition::getPosition() const {
    return position;
}

void ContainerPosition::setPosition(const std::tuple<int, int, int> &position) {
    ContainerPosition::position = position;
}
