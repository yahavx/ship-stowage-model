//
// Created by yahav on 05/04/2020.
//

#include "ContainerShip.h"

std::vector<PackingOperation>
ContainerShip::dock(const PortId &portId, const std::vector<Container> &containersToLoad) {
    std::vector<PackingOperation> operations = std::vector<PackingOperation>();

    std::vector<ContainerPosition> containersToUnload = this->getCargo().getContainersForPort(portId);

    //Unload all required containers
    for(const ContainerPosition &containerPos: containersToUnload) {
        //Get instructions for removing the container
        std::vector<PackingOperation> unloadOps = this->unloadContainer(containerPos);

        //Add unload operations  to set of all instructions
        operations.insert(operations.end(), unloadOps.begin(), unloadOps.end());
    }

    //Load all required containers , TODO: Handle case where there is no more space
    for (const Container &container: containersToLoad) {
        //Get instructions for adding the container
        std::vector<PackingOperation> loadOps = this->loadContainerToaArbitraryPosition(container);

        //Add load operations to set of all instructions
        operations.insert(operations.end(), loadOps.begin(), loadOps.end());
    }

    return operations;
}

std::vector<PackingOperation> ContainerShip::loadContainerToaArbitraryPosition(const Container &container) {
    return std::vector<PackingOperation>();
}

std::vector<PackingOperation> ContainerShip::unloadContainer(const ContainerPosition &container) {
    return std::vector<PackingOperation>();
}

const ShipPlan &ContainerShip::getShipPlan() const {
    return shipPlan;
}

void ContainerShip::setShipPlan(const ShipPlan &shipPlan) {
    ContainerShip::shipPlan = shipPlan;
}

Cargo &ContainerShip::getCargo() {
    return this->balanceCalculator.getCargo();
}

void ContainerShip::setCargo(const Cargo &cargo) {

}

const ShipRoute &ContainerShip::getShipRoute() const {
    return shipRoute;
}

void ContainerShip::setShipRoute(const ShipRoute &shipRoute) {
    ContainerShip::shipRoute = shipRoute;
}

const WeightBalanceCalculator &ContainerShip::getBalanceCalculator() const {
    return balanceCalculator;
}

void ContainerShip::setBalanceCalculator(const WeightBalanceCalculator &balanceCalculator) {
//    ContainerShip::balanceCalculator = balanceCalculator;
}
