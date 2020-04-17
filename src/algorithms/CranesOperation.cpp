//
// Created by Orr on 4/17/2020.
//

#include "CranesOperation.h"

CraneOperationResult preformLoadOperation(const PackingOperation& op, Port &port, ContainerShip &ship) {
    const Container &container = port.removeContainer(op.getContainerId());
    //TODO: handle container not found and return CraneOperationResult ::FAIL_CONTAINER_NOT_FOUND
    std::tuple<int, int, int> pos = op.getIndices();
    int result = ship.getCargo().addContainerOnTop(std::get<0>(pos), std::get<1>(pos), container);

    //If load load to ship failed, put container back at port and return false
    if(result < 0) {
        port.addContainer(container);
        return CraneOperationResult ::FAIL_ILLEGAL_OP;
    }

    return CraneOperationResult::SUCCESS;
}

CraneOperationResult preformUnloadOperation(const PackingOperation& op, Port &port, ContainerShip &ship) {
    std::tuple<int, int, int> pos = op.getIndices();
    const Container &container = ship.getCargo().getTopContainer(std::get<0>(pos), std::get<1>(pos));
    if(container.getId() != op.getContainerId()) {
        return CraneOperationResult::FAIL_ILLEGAL_OP;
    }

    ship.getCargo().removeTopContainer(std::get<0>(pos), std::get<1>(pos));
    port.addContainer(container);

    return CraneOperationResult ::SUCCESS;
}

CraneOperationResult CranesOperation::preformOperation(const PackingOperation& op, Port &port, ContainerShip &ship) {
    switch(op.getType()) {
        case PackingType ::load : return preformLoadOperation(op, port, ship);
        case PackingType ::unload : return preformUnloadOperation(op, port, ship);
        default: return CraneOperationResult :: FAIL_ILLEGAL_OP;
    }
}


