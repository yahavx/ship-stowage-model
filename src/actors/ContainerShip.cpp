//
// Created by yahav on 05/04/2020.
//

#include "ContainerShip.h"

std::vector<PackingOperation>
ContainerShip::dock(const PortId &portId, const std::vector<Container> &containersToLoad) {
    return std::vector<PackingOperation>();
}

const ShipPlan &ContainerShip::getShipPlan() const {
    return shipPlan;
}

void ContainerShip::setShipPlan(const ShipPlan &shipPlan) {
    ContainerShip::shipPlan = shipPlan;
}

const Cargo &ContainerShip::getCargo() const {
    return cargo;
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
    ContainerShip::balanceCalculator = balanceCalculator;
}