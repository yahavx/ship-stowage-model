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

bool AlgorithmValidation::validateLoadOperation(const PackingOperation &op) {
    auto &pos = op.getFirstPosition();
    int x = pos.X(), y = pos.Y(), z = pos.floor();

    const auto &containerId = op.getContainerId();
    if (!currentPort.hasContainer(containerId)) {  // Container is in port reject list, or never was on port
        if (isBadContainer(containerId)) {
            errors.addError({ErrorFlag::AlgorithmError_TriedToLoadButShouldReject, containerId, currentPort.getId()});
        } else {
            errors.addError({ErrorFlag::AlgorithmError_ContainerIdNotExistsOnPort, op.getContainerId()});
        }
        return false;
    }

    Container container = *currentPort.getContainer(containerId);  // We can safely assume its not null

    if (ship.getCargo().hasContainer(containerId)) {
        errors.addError({ErrorFlag::AlgorithmError_ContainerIdAlreadyOnShip, op.getContainerId()});
        return false;
    }

    // Check if it is possible to load container to given position
    if (!ship.getCargo().canLoadContainerToPosition(x, y)) {
        errors.addError({ErrorFlag::AlgorithmError_LoadAboveNotLegal, op.getContainerId(), std::to_string(x), std::to_string(y)});
        return false;
    }

    int currentHeight = ship.getCargo().currentTopHeight(x, y);

    if (currentHeight != z) {
        errors.addError({ErrorFlag::AlgorithmError_LoadInvalidFloor, op.getContainerId(), std::to_string(x), std::to_string(y), std::to_string(z)});
        return false;
    }

    // Check that it is approved by the weight balancer
    auto calculatorResult = ship.getBalanceCalculator().tryOperation('L', container.getWeight(), pos.X(), pos.Y());
    if (calculatorResult != WeightBalanceCalculator::APPROVED) {
        errors.addError({ErrorFlag::AlgorithmError_WeightBalancerRejectedOperation, "Load", containerId});
        return false;
    }

    // Success

    for (longUInt i = 0; i < temporaryContainersOnPort.size(); i++) {  // If this container was unloaded from ship before, mark we loaded him back
        if (temporaryContainersOnPort[i] == containerId) {
            temporaryContainersOnPort.erase(temporaryContainersOnPort.begin() + i);
            break;
        }
    }

    return true;
}

bool AlgorithmValidation::validateUnloadOperation(const PackingOperation &op) {
    auto &pos = op.getFirstPosition();
    int x = pos.X(), y = pos.Y(), z = pos.floor();

    int currentHeight = ship.getCargo().currentTopHeight(x, y);

    if (currentHeight != z + 1) {
        errors.addError({ErrorFlag::AlgorithmError_UnloadBadPosition, op.getContainerId(), std::to_string(x), std::to_string(y)});
        return false;
    }

    auto containerOptional = ship.getCargo().getTopContainer(pos.X(), pos.Y());
    if (!containerOptional.has_value()) { // There are no containers at given (x,y)
        errors.addError({ErrorFlag::AlgorithmError_UnloadNoContainersAtPosition, op.getContainerId(), std::to_string(x), std::to_string(y)});
        return false;
    }

    auto container = containerOptional.value();
    if (container.getId() != op.getContainerId()) { // The top container at given (x,y) has different id
        errors.addError({ErrorFlag::AlgorithmError_UnloadBadId, op.getContainerId(), std::to_string(x), std::to_string(y)});
        return false;
    }

    const std::string &containerId = op.getContainerId();
//    Container container = *ship.getCargo().getContainerById(containerId);  // We already get it from getTopContainer - check maybe verify this return the sames one (sanity check)

    // Check that it is approved by the weight balancer
    auto calculatorResult = ship.getBalanceCalculator().tryOperation('U', container.getWeight(), pos.X(), pos.Y());
    if (calculatorResult != WeightBalanceCalculator::APPROVED) {
        errors.addError({ErrorFlag::AlgorithmError_WeightBalancerRejectedOperation, "Unload", containerId});
        return false;
    }

    // Success

    if (container.getDestPort() != currentPort.getId()) {  // This container is not for this port, track to make sure we load him back later
        temporaryContainersOnPort.push_back(container.getId());
    }

    return true;
}

bool AlgorithmValidation::validateMoveOperation(const PackingOperation &op) {
    const Position &unloadFrom = op.getFirstPosition();
    int x = unloadFrom.X(), y = unloadFrom.Y();

    auto containerOptional = ship.getCargo().getTopContainer(unloadFrom.X(), unloadFrom.Y());

    // Check if can remove the container from it's current position
    if (!containerOptional.has_value()) { // There are no containers at given (x,y)
        errors.addError({ErrorFlag::AlgorithmError_MoveNoContainersAtPosition, op.getContainerId(), std::to_string(x), std::to_string(y)});
        return false;
    }

    auto container = containerOptional.value();
    if (container.getId() != op.getContainerId()) { // The top container at given (x,y) has different id
        errors.addError({ErrorFlag::AlgorithmError_MoveBadId, std::to_string(x), std::to_string(y), container.getId(), op.getContainerId()});
        return false;
    }

    auto loadTo = op.getFirstPosition();

    // Check if it is possible to add container to given position
    x = loadTo.X(), y = loadTo.Y();
    if (!ship.getCargo().canLoadContainerToPosition(x, y)) {
        errors.addError({ErrorFlag::AlgorithmError_MoveAboveNotLegal, op.getContainerId(), std::to_string(x), std::to_string(y)});
        return false;
    }

    const std::string &containerId = op.getContainerId();
//    Container container = *ship.getCargo().getContainerById(containerId);

    // Check that it is approved by the weight balancer
    auto calculatorResult = ship.getBalanceCalculator().tryOperation('U', container.getWeight(), unloadFrom.X(), unloadFrom.Y());
    if (calculatorResult != WeightBalanceCalculator::APPROVED) {
        errors.addError({ErrorFlag::AlgorithmError_WeightBalancerRejectedOperation, "Move (unload)", containerId});
        return false;
    }

    // If unloading the container is legal, temporarily remove it and validate loading to new position
    ship.getCargo().removeTopContainer(unloadFrom.X(), unloadFrom.Y());

    bool success = true;

    // Check that it is approved by the weight balancer
    calculatorResult = ship.getBalanceCalculator().tryOperation('L', container.getWeight(), loadTo.X(), loadTo.Y());
    if (calculatorResult != WeightBalanceCalculator::APPROVED) {
        errors.addError({ErrorFlag::AlgorithmError_WeightBalancerRejectedOperation, "Move (load)", containerId});
        success = false;
    }

    // Load back the container we unloaded - in any case (error or not)
    ship.getCargo().loadContainerOnTop(unloadFrom.X(), unloadFrom.Y(), container);
    return success;
}

bool AlgorithmValidation::validateRejectOperation(const PackingOperation &op) {
    auto &containerId = op.getContainerId();

    for (longUInt i = 0; i < badContainerIds.size(); i++) {
        if (badContainerIds[i] == containerId) {  // Successful reject
            badContainerIds.erase(badContainerIds.begin() + i);
            return true;
        }
    }

    // If ship is full then the reject is valid
    if (this->ship.getCargo().isFull()) {
        errors.addError({ErrorFlag::ContainersAtPort_ContainersExceedsShipCapacity, containerId});
        return true;  // its not an algorithm fault
    }

    // Rejection failed, need to find out why
    if (currentPort.hasContainer(containerId)) {  // Its a legal one
        errors.addError({ErrorFlag::AlgorithmError_RejectedGoodContainer, containerId});
    } else {  // This container doesn't exists
        errors.addError({ErrorFlag::AlgorithmError_ContainerIdNotExistsOnPort, containerId});
    }

    return false;
}

bool AlgorithmValidation::validatePackingOperation(const PackingOperation &op) {
    const auto &pos1 = op.getFirstPosition(), pos2 = op.getSecondPosition();
    auto opType = op.getType();

    // Validate the positions are legal
    if (opType != PackingType::reject) {
        if (!validatePosition(pos1)) {
            int x = pos1.X(), y = pos1.Y();
            errors.addError({ErrorFlag::AlgorithmError_InvalidXYCoordinates, op.getContainerId(), std::to_string(x), std::to_string(y)});
            return false;
        }
        if (opType == PackingType::move && !validatePosition(pos2)) {
            int x = pos2.X(), y = pos2.Y();
            errors.addError({ErrorFlag::AlgorithmError_InvalidXYCoordinates, op.getContainerId(), std::to_string(x), std::to_string(y)});
            return false;
        }
    }

    // Validate the operation
    switch (opType) {
        case PackingType::load:
            return validateLoadOperation(op);
        case PackingType::unload:
            return validateUnloadOperation(op);
        case PackingType::move:
            return validateMoveOperation(op);
        case PackingType::reject:
            return validateRejectOperation(op);
    }

    return false;  // Unreachable
}

bool AlgorithmValidation::validateNoContainersLeftOnPort() {
    if (!temporaryContainersOnPort.empty()) {
        for (auto &containerId : temporaryContainersOnPort) {
            errors.addErrorReport(ErrorFlag::AlgorithmError_UnloadedAndDidntLoadBack, containerId);
        }
        return false;
    }

    if (ship.getCargo().isFull()) {  // We can't load anymore
        return true;
    }

    bool success = true;

    // Ship is not full, check port is empty
    for (auto &portId : ship.getShipRoute().getNextPortsSet()) {
        PortId id(portId);
        if (id == currentPort.getId())
            continue;
        const auto &containersLeftOnPort = currentPort.getContainersForDestination(id);
        if (!containersLeftOnPort.empty()) {
            for (auto &container : containersLeftOnPort) {
                errors.addError({ErrorFlag::AlgorithmError_LeftContainersAtPort, container.getId(), portId, currentPort.getId()});
            }
            success = false;
        }
    }

    return success;
}

bool AlgorithmValidation::validateNoContainersLeftOnShip() {
    std::vector<ContainerPosition> containersForPort = ship.getCargo().getContainersForPort(currentPort.getId());

    if (!containersForPort.empty()) {
        for (ContainerPosition &container: containersForPort) {
            errors.addError({AlgorithmError_LeftContainersAtShip, container.getContainer().getId(), currentPort.getId()});
        }

        return false;
    }

    return true;
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
