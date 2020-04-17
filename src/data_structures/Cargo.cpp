//
// Created by Orr on 4/16/2020.
//

#include "Cargo.h"
#include "../algorithms/WeightBalanceCalculator.h"

//TODO: just to have something to return, can't return null reference
static Container tmpContainer (100, PortId("id"), "id");

const Container &Cargo::getTopContainer(int x, int y) {
    return tmpContainer;
}

const Container &Cargo::removeTopContainer(int x, int y) {
    return tmpContainer;
}

int Cargo::canLoadContainerOnTop(int x, int y, const Container &container) const {
    return 0;
}

int Cargo::loadContainerOnTop(int x, int y, const Container &container) {
    return 0;
}

std::vector<ContainerPosition> Cargo::getContainersForPort(const PortId &portId) const {
    return std::vector<ContainerPosition>();
}

int Cargo::currentHeight(int x, int y) const {
    return 0;
}


