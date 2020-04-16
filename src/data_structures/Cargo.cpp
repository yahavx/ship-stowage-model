//
// Created by Orr on 4/16/2020.
//

#include "Cargo.h"

const Container &Cargo::getTopContainer(int x, int y) {
    return Container(100, PortId("id"), "id");
}

const Container &Cargo::removeTopContainer(int x, int y) {
    return Container(100, PortId("id"), "id");
}

int Cargo::addContainerOnTop(int x, int y, const Container &container) {
    return false;
}

std::vector<ContainerPosition> Cargo::getContainersForPort(PortId &portId) {
    return std::vector<ContainerPosition>();
}
