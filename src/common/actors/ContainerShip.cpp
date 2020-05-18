//
// Created by yahav on 05/04/2020.
//

#include "ContainerShip.h"
#include "CranesManagement.h"
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

//void ContainerShip::setCargo(const Cargo &cargo) {
//    ContainerShip::cargo = cargo;
//}

WeightBalanceCalculator &ContainerShip::getBalanceCalculator() const {
    return *balanceCalculator;
}

void ContainerShip::setBalanceCalculator(WeightBalanceCalculator &balanceCalculator) {
    ContainerShip::balanceCalculator = &balanceCalculator;
}

// endregion

// region Functions

void ContainerShip::advanceToNextPort() {
    if (!shipRoute.getPorts().empty()) {
        this->shipRoute.getPorts().erase(this->shipRoute.getPorts().begin());
    }
}

PortId ContainerShip::getCurrentPortId() {
    if (!shipRoute.getPorts().empty()) {
        return shipRoute.getPorts()[0];
    }
    return PortId("ERROR");
}

Operations ContainerShip::unloadContainer(Port &port, const ContainerPosition &containerPos) {
    Operations ops;
    auto x = containerPos.x(), y = containerPos.y(), z = containerPos.z();
    int currentHeight = this->getCargo().currentTopHeight(x, y);
    int numOfContainersOnTop = currentHeight - z - 1;
    CranesManagement crane(*this, port);

    Containers containersOnTop = Containers();

    // Failed is true if some part of the unloading operation is not possible
    bool failed = false;

    // Unload all containers on top, later we will load them back
    for (int i = 0; i < numOfContainersOnTop; i++) {
        auto topCont = this->getCargo().getTopContainer(x, y);
        WeightBalanceCalculator::BalanceStatus status = this->balanceCalculator->tryOperation('U', topCont->getWeight(), x, y);
        if (status != WeightBalanceCalculator::BalanceStatus::APPROVED) {
            failed = true;
            break;
        }

        auto containerOptional = this->getCargo().getTopContainer(x, y);
        if (!containerOptional.has_value()) {
//            std::cout << "Error unloading container, could not get top container from cargo (" << containerPos.x() << ", " << containerPos.y() << ")" << std::endl;
            failed = true;
            break;
        }

        auto container = containerOptional.value();

        // Add to list of containers to load back only if the container destination is not current port
        containersOnTop.push_back(container);
        auto op = PackingOperation(PackingType::unload, container.getId(), {x, y, z + (numOfContainersOnTop - i)});
        auto result = crane.preformOperation(op);
        if (result != CraneOperationResult::SUCCESS) {
//            std::cout << "Error unloading container, crane operation failed to unload container on top of required one: " << op << "\n";

            failed = true;
            break;
        }
        ops.addOperation(op);
    }

    // Unload the requested container
    auto containerOptional = this->getCargo().getTopContainer(x, y);
    if (!containerOptional.has_value()) {
//        std::cout << "Error unloading container, could not get top container from cargo ("
//                  << containerPos.x() << ", " << containerPos.y() << ")" << std::endl;
        failed = true;
    } else {
        auto container = containerOptional.value();
        WeightBalanceCalculator::BalanceStatus status = this->balanceCalculator->tryOperation('U', container.getWeight(), x, y);
        if (status == WeightBalanceCalculator::BalanceStatus::APPROVED) {
            auto op = PackingOperation(PackingType::unload, container.getId(), {x, y, z});
            auto result = crane.preformOperation(op);
            if (result == CraneOperationResult::SUCCESS) {
                ops.addOperation(op);
            } else { // CranesOperation failed
                std::cout
//                        << "Error unloading container, crane operation failed to unload requested container: "
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
        // TODO: check if balance calculator allows to load back, if not load to another place
        auto op = PackingOperation(PackingType::load, cont.getId(), {x, y, static_cast<int>(z + i)});
        crane.preformOperation(op);
        ops.addOperation(op);
    }

    // If failed return reject packing operation
    if (failed) {
        ops = Operations();
        ops.addOperation(PackingOperation(PackingType::reject, containerPos.getContainer().getId()));
        return ops;
    }

    return ops;
}

OptionalContainer ContainerShip::getContainerById(const std::string &id) {
    return cargo.getContainerById(id);
}

bool ContainerShip::isContainerOnShip(const std::string &containerId) {
    return cargo.hasContainer(containerId);
}

Operations ContainerShip::loadContainer(LoadContainerStrategy *strategy, Port &port, const Container &container, Errors &errors) {
    return strategy->loadContainer(port, *this, container, errors);
}

StringVector ContainerShip::filterContainers(const Containers &containersToLoad, Errors &errors) {
    std::unordered_set<std::string> portsSet = this->shipRoute.getNextPortsSet();  // All next ports in the route
    StringVector invalidContainersIds;

    for (std::size_t i = 0; i < containersToLoad.size(); i++) {
        auto &container = containersToLoad[i];

        auto &contId = container.getId();


        if (this->cargo.hasContainer(contId)) {  // Already seen this id
            errors.addError({ErrorFlag::ContainersAtPort_IDAlreadyOnShip, contId});
            invalidContainersIds.push_back(contId);
#ifdef DEBUG_PRINTS
            std::cout << "DUPLICATE ID: " << contId << std::endl;
#endif
            continue;
        }

        Error err = container.isContainerLegal();

        // Illegal containers
        if (!err.isSuccess()) {
            errors.addError(err);
            invalidContainersIds.push_back(contId);
            continue;
        }

        // Container destination is not on route
        if (portsSet.find(container.getDestPort()) == portsSet.end()) {
            errors.addError({ContainersAtPort_ContainerNotOnRoute, container.getId(), container.getDestPort()});
            invalidContainersIds.push_back(contId);
        }
    }

    return invalidContainersIds;
}


// endregion