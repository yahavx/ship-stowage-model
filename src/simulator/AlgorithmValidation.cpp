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

bool validatePosition(POS pos, ContainerShip &ship) {
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
    }

    if (ship.getCargo().hasContainer(containerId)) {
        errors.addError({ErrorFlag::AlgorithmError_ContainerIdAlreadyOnShip, op.getContainerId()});
    }

    auto pos = op.getFirstPosition();
    //Check if it is possible to load container to given position
    if (ship.getCargo().canLoadContainerOnTop(std::get<0>(pos), get<1>(pos))) {
        int x = std::get<0>(pos), y = std::get<1>(pos);
        errors.addError({ErrorFlag::AlgorithmError_LoadAboveNotLegal, op.getContainerId(), std::to_string(x), std::to_string(y)});
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
    if (ship.getCargo().canLoadContainerOnTop(std::get<0>(loadTo), get<1>(loadTo))) {
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

    // Rejection failed, need to find out why

    if (currentPort.hasContainer(containerId)) {  // Its a legal one
        errors.addError({ErrorFlag::AlgorithmError_RejectedGoodContainer, containerId});
    } else {  // This container doesn't exists
        errors.addError({ErrorFlag::AlgorithmError_ContainerIdNotExistsOnPort, containerId});
    }
}

void AlgorithmValidation::validatePackingOperation(const PackingOperation &op) {

    auto pos = op.getFirstPosition();
    if (!validatePosition(pos, ship)) {
        int x = std::get<0>(pos), y = std::get<1>(pos);
        errors.addError({ErrorFlag::AlgorithmError_InvalidXYCoordinates, op.getContainerId(), std::to_string(x), std::to_string(y)});
    }

    switch (op.getType()) {
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
    for (auto &portId : ship.getShipRoute().getNextPortsSet()) {
        PortId id(portId);
        if (id == currentPort.getId())
            continue;
        if (!currentPort.getContainersForDestination(id).empty() && !ship.getCargo().isFull()) {
            errors.addError({ErrorFlag::AlgorithmError_LeftContainersAtPort, portId, currentPort.getId()});
        }
    }
}

bool AlgorithmValidation::isBadContainer(const std::string &id) {
    for (auto& badId: badContainerIds) {
        if (badId == id) {
            return true;
        }
    }

    return false;
}

// endregion
