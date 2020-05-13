//
// Created by t-yabeny on 5/6/2020.
//

#include "AlgorithmValidation.h"
#include <vector>

// region Constructor

AlgorithmValidation::AlgorithmValidation(ContainerShip &ship, Port &currentPort, StringVector &badContainerIds, Errors &errors) : ship(ship),
                                                                                                                                  currentPort(currentPort),
                                                                                                                                  badContainerIds(
                                                                                                                                          badContainerIds),
                                                                                                                                  errors(errors) {}

// endregion

// region Validations

bool AlgorithmValidation::validatePosition(const Position &pos) {
    int x = pos.X(), y = pos.Y();
    auto &dims = ship.getShipPlan().getDimensions();
    return x >= 0 && x < dims.X() && y >= 0 && y < dims.Y();
}

void AlgorithmValidation::validateLoadOperation(const PackingOperation &op) {
    const auto &containerId = op.getContainerId();
    if (!currentPort.hasContainer(containerId)) {
        if (isBadContainer(containerId)) {
            errors.addError({ErrorFlag::AlgorithmError_TriedToLoadButShouldReject, containerId, currentPort.getId()});
        } else {
            errors.addError({ErrorFlag::AlgorithmError_ContainerIdNotExistsOnPort, op.getContainerId()});
        }
        return;
    }

    Container container = *currentPort.getContainer(containerId);  // we can safely assume its not null

    if (ship.getCargo().hasContainer(containerId)) {
        errors.addError({ErrorFlag::AlgorithmError_ContainerIdAlreadyOnShip, op.getContainerId()});
        return;
    }

    auto pos = op.getFirstPosition();
    // Check if it is possible to load container to given position
    if (!ship.getCargo().canLoadContainerToPosition(pos.X(), pos.Y())) {
        int x = pos.X(), y = pos.Y();
        errors.addError({ErrorFlag::AlgorithmError_LoadAboveNotLegal, op.getContainerId(), std::to_string(x), std::to_string(y)});
        return;
    }

    // Check that it is approved by the weight balancer
    if (ship.getBalanceCalculator().tryOperation('L', container.getWeight(), op.getFirstPositionX(), op.getFirstPositionY())) {
        errors.addError({ErrorFlag::AlgorithmError_WeightBalancerRejectedOperation, "Load", containerId});
        return;
    }

    // Success

    for (longUInt i = 0; i < temporaryContainersOnPort.size(); i++) {  // If this container was unloaded from ship before, mark we loaded him back
        if (temporaryContainersOnPort[i] == containerId) {
            temporaryContainersOnPort.erase(temporaryContainersOnPort.begin() + i);
            break;
        }
    }
}

void AlgorithmValidation::validateUnloadOperation(const PackingOperation &op) {
    auto &pos = op.getFirstPosition();
    int x = pos.X(), y = pos.Y(), z = pos.Z();

    int currentHeight = ship.getCargo().currentTopHeight(x, y);

    if (currentHeight != z + 1) {
        errors.addError({ErrorFlag::AlgorithmError_UnloadBadPosition, op.getContainerId(), std::to_string(x), std::to_string(y)});
        return;
    }

    auto containerOptional = ship.getCargo().getTopContainer(pos.X(), pos.Y());
    if (!containerOptional.has_value()) { // There are no containers at given (x,y)
        errors.addError({ErrorFlag::AlgorithmError_UnloadNoContainersAtPosition, op.getContainerId(), std::to_string(x), std::to_string(y)});
    } else {
        auto container = containerOptional.value();
        if (container.getId() != op.getContainerId()) { // The top container at given (x,y) has different id
            errors.addError({ErrorFlag::AlgorithmError_UnloadBadId, op.getContainerId(), std::to_string(x), std::to_string(y)});
        } else if (container.getDestPort() != currentPort.getId()) {  // This container is not for this port, track to make sure we load him back later
            temporaryContainersOnPort.push_back(container.getId());
        }
    }

    const std::string& containerId = op.getContainerId();
    Container container = *currentPort.getContainer(containerId);

    // Check that it is approved by the weight balancer
    if (ship.getBalanceCalculator().tryOperation('U', container.getWeight(), op.getFirstPositionX(), op.getFirstPositionY())) {
        errors.addError({ErrorFlag::AlgorithmError_WeightBalancerRejectedOperation, "Unload", containerId});
    }
}

void AlgorithmValidation::validateMoveOperation(const PackingOperation &op) {
    const Position& unloadFrom = op.getFirstPosition();
    int x = unloadFrom.X(), y = unloadFrom.Y();

    auto containerOptional = ship.getCargo().getTopContainer(unloadFrom.X(), unloadFrom.Y());

    // Check if can remove the container from it's current position
    if (!containerOptional.has_value()) { // There are no containers at given (x,y)
        errors.addError({ErrorFlag::AlgorithmError_MoveNoContainersAtPosition, op.getContainerId(), std::to_string(x), std::to_string(y)});
    } else {
        auto container = containerOptional.value();
        if (container.getId() != op.getContainerId()) { // The top container at given (x,y) has different id
            errors.addError({ErrorFlag::AlgorithmError_MoveBadId, op.getContainerId(), std::to_string(x), std::to_string(y)});
        }
    }

    auto loadTo = op.getFirstPosition();

    // Check if it is possible to add container to given position
    x = loadTo.X(), y = loadTo.Y();
    if (ship.getCargo().getAvailableFloorToLoadContainer(x, y)) {
        errors.addError({ErrorFlag::AlgorithmError_MoveAboveNotLegal, op.getContainerId(), std::to_string(x), std::to_string(y)});
    }

    const std::string& containerId = op.getContainerId();
    Container container = *currentPort.getContainer(containerId);

    // Check that it is approved by the weight balancer
    if (ship.getBalanceCalculator().tryOperation('U', container.getWeight(), unloadFrom.X(), unloadFrom.Y())) {
        errors.addError({ErrorFlag::AlgorithmError_WeightBalancerRejectedOperation, "Move/Unload", containerId});
        return;
    }

    // If unloading the container is legal, temporarily remove it and validate loading to new position
    ship.getCargo().removeTopContainer(unloadFrom.X(), unloadFrom.Y());

    // Check that it is approved by the weight balancer
    if (ship.getBalanceCalculator().tryOperation('L', container.getWeight(), loadTo.X(), loadTo.Y())) {
        errors.addError({ErrorFlag::AlgorithmError_WeightBalancerRejectedOperation, "Move/Load", containerId});
    }

    // Load back the container we unloaded
    ship.getCargo().loadContainerOnTop(unloadFrom.X(), unloadFrom.Y(), container);

}

void AlgorithmValidation::validateRejectOperation(const PackingOperation &op) {
    auto &containerId = op.getContainerId();

    for (longUInt i = 0; i < badContainerIds.size(); i++) {
        if (badContainerIds[i] == containerId) {  // Successful reject
            badContainerIds.erase(badContainerIds.begin() + i);
            return;
        }
    }

    // If ship is full then the reject is valid
    if (this->ship.getCargo().isFull()) {
        errors.addError({ErrorFlag::ContainersAtPort_ContainersExceedsShipCapacity, containerId});
        return;
    }

    // Rejection failed, need to find out why
    if (currentPort.hasContainer(containerId)) {  // Its a legal one
        errors.addError({ErrorFlag::AlgorithmError_RejectedGoodContainer, containerId});
    } else {  // This container doesn't exists
        errors.addError({ErrorFlag::AlgorithmError_ContainerIdNotExistsOnPort, containerId});
    }
}

void AlgorithmValidation::validatePackingOperation(const PackingOperation &op) {
    const auto& pos1 = op.getFirstPosition(), pos2 = op.getSecondPosition();
    auto opType = op.getType();

    if (opType != PackingType::reject) {
        if (!validatePosition(pos1)) {
            int x = pos1.X(), y = pos1.Y();
            errors.addError({ErrorFlag::AlgorithmError_InvalidXYCoordinates, op.getContainerId(), std::to_string(x), std::to_string(y)});
            return;
        }
        if (opType == PackingType::move && !validatePosition(pos2)) {
            int x = pos2.X(), y = pos2.Y();
            errors.addError({ErrorFlag::AlgorithmError_InvalidXYCoordinates, op.getContainerId(), std::to_string(x), std::to_string(y)});
            return;
        }
    }

    switch (opType) {
        case PackingType::load:
            validateLoadOperation(op);
            break;
        case PackingType::unload:
            validateUnloadOperation(op);
            break;
        case PackingType::move:
            validateMoveOperation(op);
            break;
        case PackingType::reject:
            validateRejectOperation(op);
            break;

    }
}

void AlgorithmValidation::validateNoContainersLeftOnPort() {
    if (!temporaryContainersOnPort.empty()) {
        errors.addError(ErrorFlag::AlgorithmError_UnloadedAndDidntLoadBack);
    }

    for (auto &portId : ship.getShipRoute().getNextPortsSet()) {
        PortId id(portId);
        if (id == currentPort.getId())
            continue;
        if (!currentPort.getContainersForDestination(id).empty() && !ship.getCargo().isFull()) {
            errors.addError({ErrorFlag::AlgorithmError_LeftContainersAtPort, portId, currentPort.getId()});
        }
    }

    // TODO: check temporaryContainersOnPort, to know if containers were not loaded, or were unloaded and not loaded back? (maybe its useless and should be removed)
}

bool AlgorithmValidation::isBadContainer(const std::string &id) {
    for (std::string badId: badContainerIds) {
        if (badId == id) {
            return true;
        }
    }

    return false;
}

// endregion
