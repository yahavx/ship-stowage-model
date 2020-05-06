//
// Created by Orr on 4/17/2020.
//

#include "CranesOperation.h"

CraneOperationResult preformLoadOperation(const PackingOperation &op, Port &port, ContainerShip &ship) {
    auto containerOptional = port.removeContainer(op.getContainerId());
    if (!containerOptional.has_value()) {
        std::cout << "Error preforming load operation, could not remove from port (container not found)" << std::endl;
        return CraneOperationResult::FAIL_CONTAINER_NOT_FOUND;
    }
    auto container = containerOptional.value();
    std::tuple<int, int, int> pos = op.getFirstPosition();
    int result = ship.getCargo().loadContainerOnTop(std::get<0>(pos), std::get<1>(pos), container);

    // If load to ship failed, put container back at port and return false
    if (result < 0) {
        port.addContainer(container);
        return CraneOperationResult::FAIL_ILLEGAL_OP;
    }

    return CraneOperationResult::SUCCESS;
}

CraneOperationResult preformUnloadOperation(const PackingOperation &op, Port &port, ContainerShip &ship) {
    std::tuple<int, int, int> pos = op.getFirstPosition();
    auto containerOptional = ship.getCargo().getTopContainer(std::get<0>(pos), std::get<1>(pos));
    if (!containerOptional.has_value()) {
        std::cout << "Error unloading container, could not remove top container from cargo ("
                  << std::get<0>(pos) << ", " << std::get<1>(pos) << ")" << std::endl;
        return CraneOperationResult::FAIL_ILLEGAL_OP;
    }
    auto container = containerOptional.value();
    if (container.getId() != op.getContainerId()) {
        return CraneOperationResult::FAIL_ILLEGAL_OP;
    }

    ship.getCargo().removeTopContainer(std::get<0>(pos), std::get<1>(pos));
    port.addContainer(container);

    return CraneOperationResult::SUCCESS;
}

CraneOperationResult CranesOperation::preformOperation(const PackingOperation &op, Port &port, ContainerShip &ship) {
    switch (op.getType()) {
        case PackingType::load :
            return preformLoadOperation(op, port, ship);
        case PackingType::unload :
            return preformUnloadOperation(op, port, ship);
        default:
            return CraneOperationResult::FAIL_ILLEGAL_OP;
    }
}


