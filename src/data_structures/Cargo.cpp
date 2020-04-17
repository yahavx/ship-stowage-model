//
// Created by Orr on 4/16/2020.
//

#include "Cargo.h"
#include "../algorithms/WeightBalanceCalculator.h"

//TODO: just to have something to return, can't return null reference
static Container tmpContainer ("id", 100 , PortId("id"));

const Container &Cargo::getTopContainer(int x, int y) {
    return tmpContainer;
}

const Container &Cargo::removeTopContainer(int x, int y) {
    return tmpContainer;
}

int Cargo::addContainerOnTop(int x, int y, const Container &container) {
    return 0;
}

std::vector<ContainerPosition> Cargo::getContainersForPort(const PortId &portId) const {
    return std::vector<ContainerPosition>();
}
