//
// Created by yahav on 05/04/2020.
//

#include "ContainerShip.h"
#include "../algorithms/CranesOperation.h"
#include "../utils/Definitions.h"

// region Constructors

ContainerShip::ContainerShip() {}

ContainerShip::ContainerShip(const ShipPlan &shipPlan, const ShipRoute &shipRoute) : shipPlan(shipPlan),
                                                                                     shipRoute(shipRoute) {}
// endregion

// region Getters and setters


const ShipPlan &ContainerShip::getShipPlan() const {
    return shipPlan;
}

void ContainerShip::setShipPlan(const ShipPlan &shipPlan) {
    ContainerShip::shipPlan = shipPlan;
}

Cargo &ContainerShip::getCargo() {
    return this->balanceCalculator.getCargo();
}

void ContainerShip::setCargo(Cargo &cargo) {
    this->balanceCalculator.setCargo(cargo);
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
// endregion

// region Functions

OPS
ContainerShip::dock(const PortId &portId, const Containers &containersToLoad) {
    OPS operations = OPS();

    std::vector<ContainerPosition> containersToUnload = this->getCargo().getContainersForPort(portId);

    // Unload all required containers
    for (const ContainerPosition &containerPos: containersToUnload) {
        // Get instructions for removing the container
        OPS unloadOps = this->unloadContainer(containerPos);

        // Add unload operations  to set of all instructions
        operations.insert(operations.end(), unloadOps.begin(), unloadOps.end());
    }

    // Load all required containers , TODO: Handle case where there is no more space
    for (const Container &container: containersToLoad) {
        // Get instructions for adding the container
        OPS loadOps = this->loadContainerToArbitraryPosition(container);

        // Add load operations to set of all instructions
        operations.insert(operations.end(), loadOps.begin(), loadOps.end());
    }

    return operations;
}

OPS ContainerShip::loadContainerToArbitraryPosition(const Container &container) {
    OPS ops = OPS();
    POS dims = this->shipPlan.getDimensions();
    int z = -1;

    /// Loop over all possible ship matrix cells and try to load the container on top, until success
    for (int x = 0; (x < std::get<0>(dims)) && (z < 0); x++) {
        for (int y = 0; (y < std::get<1>(dims)) && (z < 0); y++) {
            BalanceStatus status = this->balanceCalculator.tryOperation('L', container.getWeight(), x, y);
            if (status == BalanceStatus::APPROVED) {
                z = this->getCargo().loadContainerOnTop(x, y, container);
                if (z >= 0) /// Successfully loaded
                    ops.push_back(PackingOperation(PackingType::load, container.getId(), {x, y, z}));
            }
        }
    }

    if (z < 0) {
        ops = OPS();
        ops.push_back(PackingOperation(PackingType::reject, container.getId(), {-1, -1, -1}));
        return ops;
    }

    return ops;
}

OPS ContainerShip::unloadContainer(const ContainerPosition &containerPos) {
    OPS ops;
    int currentHeight = this->getCargo().currentNumContainers(containerPos.x(), containerPos.y());
    int numOfContainersOnTop = currentHeight - containerPos.z();

    Containers containersOnTop = Containers();

    auto x = containerPos.x();
    auto y = containerPos.y();
    auto z = containerPos.z();

    // Failed is true if some part of the unloading operation is not possible
    bool failed = false;

    // Unload all containers on top, later we will load them back
    for (int i = 0; i < numOfContainersOnTop; i++) {
        //TODO: Check if balance calculator allows to unload
        auto topCont = this->getCargo().getTopContainer(x, y);
        BalanceStatus status = this->balanceCalculator.tryOperation('U', topCont->getWeight(), x, y);
        if (status != BalanceStatus::APPROVED) {
            failed = true;
            break;
        }

        auto containerOptional = this->getCargo().removeTopContainer(x, y);
        if (!containerOptional.has_value()) {
            std::cout
                    << "Error unloading container, could not remove top container from cargo, one on top of required one ("
                    << containerPos.x() << ", " << containerPos.y() << ")" << "\n";

            failed = true;
            break;
        }

        auto container = containerOptional.value();
        containersOnTop.push_back(container);
        ops.push_back(PackingOperation(PackingType::unload, container.getId(),
                                       {containerPos.x(), containerPos.y(),
                                        containerPos.z() + (numOfContainersOnTop - i)}
        ));
    }

    // Unload the requested container
    auto topCont = this->getCargo().getTopContainer(x, y);
    BalanceStatus status = this->balanceCalculator.tryOperation('U', topCont->getWeight(), x, y);
    if (status != BalanceStatus::APPROVED) {
        failed = true;
    }

    if (!failed) { //If not failed actually remove top container
        auto containerOptional = this->getCargo().removeTopContainer(x, y);
        if (!containerOptional.has_value()) {
            std::cout << "Error unloading container, could not remove top container from cargo, the required one ("
                      << containerPos.x() << ", " << containerPos.y() << ")" << "\n";

            failed = true;
        } else {
            // Add unload op for the requested container
            auto container = containerOptional.value();
            ops.push_back(PackingOperation(PackingType::unload, container.getId(), {x, y, z}));
        }
    }

    // Load back the containers that were on top and were unloaded
    for (std::size_t i = 0, max = containersOnTop.size(); i < max; i++) {
        Container cont = containersOnTop[i];
        //TODO: check if balance calculator allows to load, if not load to another place
        this->getCargo().loadContainerOnTop(x, y, cont);
        ops.push_back(PackingOperation(PackingType::load, cont.getId(), {x, y, z + i}));
    }

    // If failed return reject packing operation
    if (failed) {
        ops = OPS();
        ops.push_back(PackingOperation(PackingType::reject, containerPos.getContainer().getId(), {-1, -1, -1}));
        return ops;
    }

    return ops;
}
// endregion