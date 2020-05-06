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

void AlgorithmValidation::validateLoadOperation(const PackingOperation &op) {
    const auto &containerId = op.getContainerId();
    if (ship.getCargo().hasContainer(containerId)) {
        errors.addError({AlgorithmError_ContainerIdAlreadyOnShip, op.getContainerId()});
    }
}

void AlgorithmValidation::validatePackingOperation(const PackingOperation &op) {

    switch (op.getType()) {
        case PackingType::load:
            validateLoadOperation(op);
            break;
        case PackingType::unload:
        case PackingType::reject:
            validateRejectOperation(op);
        case PackingType::move:
            break;
    }
}

void AlgorithmValidation::validateRejectOperation(const PackingOperation &op) {
    auto& containerId = op.getContainerId();

    for (longUInt i = 0; i < badContainerIds.size(); i++) {
        if (badContainerIds[i] == containerId) {  // Successful reject
            badContainerIds.erase(badContainerIds.begin() + i);
            return;
        }
    }

    // Rejection failed, need to find out why

    if (currentPort.hasContainer(containerId)) {  // Its a legal one
        errors.addError({AlgorithmError_RejectedGoodContainer, containerId});
    }
    else {  // This container doesn't exists
        errors.addError({AlgorithmError_ContainerIdNotExistsOnPort, containerId});
    }
}

// endregion
