//
// Created by yahav on 05/04/2020.
//

#include "ContainerShip.h"
#include "../algorithms/CranesOperation.h"
#include "../utils/Definitions.h"

// region Constructors

ContainerShip::ContainerShip() {}

ContainerShip::ContainerShip(const ShipPlan &shipPlan, const ShipRoute &shipRoute) : shipPlan(shipPlan),
                                                                                     shipRoute(shipRoute),
                                                                                     cargo(shipPlan) {}

ContainerShip::ContainerShip(const ShipPlan &shipPlan, const ShipRoute &shipRoute,
                             WeightBalanceCalculator &balanceCalculator) : shipPlan(shipPlan), shipRoute(shipRoute),
                                                                           cargo(shipPlan),
                                                                           balanceCalculator(&balanceCalculator) {}
// endregion

// region Getters and setters

const ShipPlan &ContainerShip::getShipPlan() const {
    return shipPlan;
}

void ContainerShip::setShipPlan(const ShipPlan &shipPlan) {
    ContainerShip::shipPlan = shipPlan;
    ContainerShip::cargo = Cargo(shipPlan);
}

ShipRoute &ContainerShip::getShipRoute() {
    return shipRoute;
}

void ContainerShip::setShipRoute(ShipRoute &shipRoute) {
    ContainerShip::shipRoute = shipRoute;
}

Cargo &ContainerShip::getCargo() {
    return cargo;
}

void ContainerShip::setCargo(const Cargo &cargo) {
    ContainerShip::cargo = cargo;
}

WeightBalanceCalculator &ContainerShip::getBalanceCalculator() const {
    return *balanceCalculator;
}

void ContainerShip::setBalanceCalculator(WeightBalanceCalculator &balanceCalculator) {
    ContainerShip::balanceCalculator = &balanceCalculator;
    balanceCalculator.setCargo(cargo);
}
// endregion

// region Functions

OPS
ContainerShip::dock(Port &port, const Containers &containersToLoad) {
    OPS operations;

    std::vector<ContainerPosition> containersToUnload = this->getCargo().getContainersForPort(port.getId());

    // Unload all required containers
    for (const ContainerPosition &containerPos: containersToUnload) {
        // Get instructions for removing the container
        OPS unloadOps = this->unloadContainer(port, containerPos);

        // Add unload operations  to set of all instructions
        operations.insert(operations.end(), unloadOps.begin(), unloadOps.end());
    }

    // Load all required containers , TODO: Handle case where there is no more space
    for (const Container &container: containersToLoad) {
        // Get instructions for adding the container
        OPS loadOps = this->loadContainerToArbitraryPosition(port, container);

        // Add load operations to set of all instructions
        operations.insert(operations.end(), loadOps.begin(), loadOps.end());
    }

    return operations;
}

OPS ContainerShip::loadContainerToArbitraryPosition(Port &port, const Container &container) {
    OPS ops = OPS();
    POS dims = this->shipPlan.getDimensions();
    int z = -1;

    // Loop over all possible ship matrix cells and try to load the container on top, until success
    for (int x = 0; (x < std::get<0>(dims)) && (z < 0); x++) {
        for (int y = 0; (y < std::get<1>(dims)) && (z < 0); y++) {
            BalanceStatus status = this->balanceCalculator->tryOperation('L', container.getWeight(), x, y);
            if (status == BalanceStatus::APPROVED) {
                z = this->getCargo().canLoadContainerOnTop(x, y);
                if (z >= 0) {
                    auto op = PackingOperation(PackingType::load, container.getId(), {x, y, z});
                    auto result = CranesOperation::preformOperation(op, port, *this);
                    if (result == CraneOperationResult::SUCCESS) { /// Successfully loaded
                        ops.push_back(op);
                    } else {
                        std::cout
                                << "Error loading container, crane operation failed to load container: "
                                << op << "\n";
                        z = -1;
                    }

                }
            }
        }
    }

    if (z < 0) {
        ops = OPS();
        ops.push_back(PackingOperation(PackingType::reject, container.getId(), {-1, -1, -1})
        );
        return ops;
    }

    return ops;
}

OPS ContainerShip::unloadContainer(Port &port, const ContainerPosition &containerPos) {
    OPS ops;
    auto x = containerPos.x();
    auto y = containerPos.y();
    auto z = containerPos.z();

    int currentHeight = this->getCargo().currentTopHeight(containerPos.x(), containerPos.y());
    int numOfContainersOnTop = currentHeight - z - 1;

    Containers containersOnTop = Containers();

    // Failed is true if some part of the unloading operation is not possible
    bool failed = false;

    // Unload all containers on top, later we will load them back
    for (int i = 0; i < numOfContainersOnTop; i++) {
        //TODO: Check if balance calculator allows to unload
        auto topCont = this->getCargo().getTopContainer(x, y);
        BalanceStatus status = this->balanceCalculator->tryOperation('U', topCont->getWeight(), x, y);
        if (status != BalanceStatus::APPROVED) {
            failed = true;
            break;
        }

        auto containerOptional = this->getCargo().getTopContainer(x, y);
        if (!containerOptional.has_value()) {
            std::cout
                    << "Error unloading container, could not remove top container from cargo, one on top of required one ("
                    << containerPos.x() << ", " << containerPos.y() << ")" << "\n";

            failed = true;
            break;
        }

        auto container = containerOptional.value();

        //Add to list of containers to load back only if the container destination is not current port
        //TODO : check why this is not working
//        if (container.getDestPort() != port.getId())
//            containersOnTop.push_back(container);
        containersOnTop.push_back(container);
        auto op = PackingOperation(PackingType::unload, container.getId(), {x, y, z + (numOfContainersOnTop - i)});
        auto result = CranesOperation::preformOperation(op, port, *this);
        if (result != CraneOperationResult::SUCCESS) {
            std::cout
                    << "Error unloading container, crane operation failed to unload one on top of required one: "
                    << op << "\n";

            failed = true;
            break;
        }
        ops.push_back(op);

    }

    // Unload the requested container
    auto containerOptional = this->getCargo().getTopContainer(x, y);
    if (!containerOptional.has_value()) {
        std::cout << "Error unloading container, could not get top container from cargo, the required one ("
                  << containerPos.x() << ", " << containerPos.y() << ")" << std::endl;
        failed = true;
    } else {
        auto container = containerOptional.value();
        BalanceStatus status = this->balanceCalculator->tryOperation('U', container.getWeight(), x, y);
        if (status == BalanceStatus::APPROVED) {
            auto op = PackingOperation(PackingType::unload, container.getId(), {x, y, z});
            auto result = CranesOperation::preformOperation(op, port, *this);
            if (result == CraneOperationResult::SUCCESS) {
                ops.push_back(op);
            } else { // CranesOperation failed
                std::cout
                        << "Error unloading container, crane operation failed to unload requested container: "
                        << op << std::endl;

                failed = true;
            }
        } else { // balanceCalculator did not approve
            failed = true;
        }
    }

    // Load back the containers that were on top and were unloaded, also if the operation failed
    for (std::size_t i = 0, max = containersOnTop.size(); i < max; i++) {
        Container cont = containersOnTop[i];
        //TODO: check if balance calculator allows to load, if not load to another place
        auto op = PackingOperation(PackingType::load, cont.getId(), {x, y, z + i});
        CranesOperation::preformOperation(op, port, *this);
//        this->getCargo().loadContainerOnTop(x, y, cont);
        ops.push_back(op);
    }

    // If failed return reject packing operation
    if (failed) {
        ops = OPS();
        ops.push_back(PackingOperation(PackingType::reject, containerPos.getContainer().getId(), {-1, -1, -1}));
        return ops;
    }

    return ops;
}

void ContainerShip::markCurrentVisitDone() {
    this->shipRoute.getPorts().erase(this->shipRoute.getPorts().begin());
}
// endregion