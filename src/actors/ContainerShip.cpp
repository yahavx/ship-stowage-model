//
// Created by yahav on 05/04/2020.
//

#include "ContainerShip.h"
#include "../algorithms/CranesOperation.h"
#include "../utils/Definitions.h"

OPS
ContainerShip::dock(const PortId &portId, const Containers &containersToLoad) {
    OPS operations = OPS();

    std::vector<ContainerPosition> containersToUnload = this->getCargo().getContainersForPort(portId);

    //Unload all required containers
    for (const ContainerPosition &containerPos: containersToUnload) {
        //Get instructions for removing the container
        OPS unloadOps = this->unloadContainer(containerPos);

        //Add unload operations  to set of all instructions
        operations.insert(operations.end(), unloadOps.begin(), unloadOps.end());
    }

    //Load all required containers , TODO: Handle case where there is no more space
    for (const Container &container: containersToLoad) {
        //Get instructions for adding the container
        OPS loadOps = this->loadContainerToaArbitraryPosition(container);

        //Add load operations to set of all instructions
        operations.insert(operations.end(), loadOps.begin(), loadOps.end());
    }

    return operations;
}

OPS ContainerShip::loadContainerToaArbitraryPosition(const Container &container) {
    OPS ops = OPS();
    POS dims = this->shipPlan.getDimensions();
    int z = -1;

    ///Loop over all possible ship matrix cells and try to load the container on top, until sucess
    for (int x = 0; (x < std::get<0>(dims)) && (z < 0); x++) {
        for (int y = 0; (y < std::get<1>(dims)) && (z < 0); y++) {
            BalanceStatus status = this->balanceCalculator.tryOperation('L', container.getWeight(), x, y);
            if (status == BalanceStatus::APPROVED) {
                z = this->getCargo().canLoadContainerOnTop(x, y, container);
                if (z >= 0) ///Successfully loaded
                    ops.push_back(PackingOperation(container.getId(), {x, y, z}, PackingType::load));
            }
        }
    }

    return ops;
}

OPS ContainerShip::unloadContainer(const ContainerPosition &containerPos) {
    OPS ops = OPS();
    int currentHeight = this->getCargo().currentHeight(containerPos.x(), containerPos.y());
    int numOfContainersOnTop = currentHeight - containerPos.z();

    Containers containersOnTop = Containers();

    ///Unload all containers on top, later we will load them back
    for (int i = 0; i < numOfContainersOnTop; i++) {
        //TODO: Check if balance calculator allows to unload
        Container container = this->getCargo().removeTopContainer(containerPos.x(), containerPos.y());
        containersOnTop.push_back(container);
        ops.push_back(PackingOperation(container.getId(),
                                       {containerPos.x(), containerPos.y(),
                                        containerPos.z() + (numOfContainersOnTop - i)},
                                       PackingType::unload));
    }

    ///Unload the specified container
    Container container = this->getCargo().removeTopContainer(containerPos.x(), containerPos.y());
    containersOnTop.push_back(container);

    ///Load back the containers that were on top
    for (int i = 0; i < containersOnTop.size(); i++) {
        Container cont = containersOnTop[i];
        //TODO: check if balance calculator allows to load, if not load to another place
        this->getCargo().loadContainerOnTop(containerPos.x(), containerPos.y(), cont);
        ops.push_back(PackingOperation(container.getId(),
                                       {containerPos.x(), containerPos.y(), containerPos.z() + i},
                                       PackingType::load));
    }

    return ops;
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

