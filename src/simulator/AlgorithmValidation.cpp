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

bool AlgorithmValidation::validatePosition(POS pos) {
    return std::get<0>(pos) >= 0 &&
           std::get<0>(pos) < std::get<0>(ship.getShipPlan().getDimensions()) &&
           std::get<1>(pos) >= 0 &&
           std::get<1>(pos) < std::get<1>(ship.getShipPlan().getDimensions());
}

void AlgorithmValidation::validateLoadOperation(const PackingOperation &op) {
    const auto &containerId = op.getContainerId();
    if (!currentPort.hasContainer(containerId)) {
        if (isBadContainer(containerId)) {
            errors.addError({ErrorFlag::AlgorithmError_TriedToLoadButShouldReject, containerId, currentPort.getId()});
        }
        else {
            errors.addError({ErrorFlag::AlgorithmError_ContainerIdNotExistsOnPort, op.getContainerId()});
        }
        return;
    }

    if (ship.getCargo().hasContainer(containerId)) {
        errors.addError({ErrorFlag::AlgorithmError_ContainerIdAlreadyOnShip, op.getContainerId()});
        return;
    }

    auto pos = op.getFirstPosition();
    // Check if it is possible to load container to given position
    if (!ship.getCargo().canLoadContainerToPosition(std::get<0>(pos), std::get<1>(pos))) {
        int x = std::get<0>(pos), y = std::get<1>(pos);
        errors.addError({ErrorFlag::AlgorithmError_LoadAboveNotLegal, op.getContainerId(), std::to_string(x), std::to_string(y)});
        return;
    }

    for (longUInt i = 0; i < temporaryContainersOnPort.size(); i++) {  // If this container was unloaded from ship before, mark we loaded him back
        if (temporaryContainersOnPort[i] == containerId) {
            temporaryContainersOnPort.erase(temporaryContainersOnPort.begin() + i);
            break;
        }
    }
}

void AlgorithmValidation::validateUnloadOperation(const PackingOperation &op) {
    std::tuple<int, int, int> pos = op.getFirstPosition();
    int x = std::get<0>(pos), y = std::get<1>(pos);
    auto containerOptional = ship.getCargo().getTopContainer(std::get<0>(pos), std::get<1>(pos));

    if (!containerOptional.has_value()) { // There are no containers at given (x,y)
        errors.addError({ErrorFlag::AlgorithmError_UnloadNoContainersAtPosition, op.getContainerId(), std::to_string(x), std::to_string(y)});
    } else {
        auto container = containerOptional.value();
        if (container.getId() != op.getContainerId()) { // The top container at given (x,y) has different id
            errors.addError({ErrorFlag::AlgorithmError_UnloadBadId, op.getContainerId(), std::to_string(x), std::to_string(y)});
        }

        else if (container.getDestPort() != currentPort.getId()) {  // This container is not for this port, track to make sure we load him back later
            temporaryContainersOnPort.push_back(container.getId());
        }
    }
}

void AlgorithmValidation::validateMoveOperation(const PackingOperation &op) {
    std::tuple<int, int, int> unloadFrom = op.getFirstPosition();
    int x = std::get<0>(unloadFrom), y = std::get<1>(unloadFrom);

    auto containerOptional = ship.getCargo().getTopContainer(std::get<0>(unloadFrom), std::get<1>(unloadFrom));

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
    if (ship.getCargo().getAvailableFloorToLoadContainer(std::get<0>(loadTo), std::get<1>(loadTo))) {
        x = std::get<0>(loadTo), y = std::get<1>(loadTo);
        errors.addError({ErrorFlag::AlgorithmError_MoveAboveNotLegal, op.getContainerId(), std::to_string(x), std::to_string(y)});
    }
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
    if(this->ship.getCargo().isFull()) {
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
    auto pos1 = op.getFirstPosition(), pos2 = op.getSecondPosition();
    auto opType = op.getType();

    if (opType != PackingType::reject) {
        if (!validatePosition(pos1)) {
            int x = std::get<0>(pos1), y = std::get<1>(pos1);
            errors.addError({ErrorFlag::AlgorithmError_InvalidXYCoordinates, op.getContainerId(), std::to_string(x), std::to_string(y)});
            return;
        }
        if (opType == PackingType::move && !validatePosition(pos2)) {
            int x = std::get<0>(pos2), y = std::get<1>(pos2);
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
