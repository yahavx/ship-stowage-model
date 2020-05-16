//
// Created by Orr on 4/16/2020.
//

#include "ContainerPosition.h"


// region Constructor

ContainerPosition::ContainerPosition(Container &container, const Position &position) : container(
        container), position(position) {}

// endregion

// region Getters and setters

Container &ContainerPosition::getContainer() const {
    return container;
}

void ContainerPosition::setContainer(Container &container) {
    ContainerPosition::container = container;
}

const Position &ContainerPosition::getPosition() const {
    return position;
}

void ContainerPosition::setPosition(const Position &position) {
    ContainerPosition::position = position;
}

int ContainerPosition::x() const {
    return position.X();
}

int ContainerPosition::y() const {
    return position.Y();
}

int ContainerPosition::z() const {
    return position.floor();
}

// endregion