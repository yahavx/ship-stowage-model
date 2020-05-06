//
// Created by Orr on 4/17/2020.
//

#include "CranesManagement.h"

// region Constructor

CranesManagement::CranesManagement(ContainerShip &ship, Port &currentPort) : ship(ship), currentPort(currentPort) {}

// endregion

// region Functions

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

CraneOperationResult preformMoveOperation(const PackingOperation &op, ContainerShip &ship) {
    std::tuple<int, int, int> unloadFrom = op.getFirstPosition();
    std::tuple<int, int, int> loadTo = op.getSecondPosition();
    auto containerOptional = ship.getCargo().getTopContainer(std::get<0>(unloadFrom), std::get<1>(unloadFrom));

    // If can't remove container do nothing and return error
    if (!containerOptional.has_value()) {
        std::cout << "Error moving container, could not remove top container from cargo ("
                  << std::get<0>(unloadFrom) << ", " << std::get<1>(unloadFrom) << ")" << std::endl;
        return CraneOperationResult::FAIL_ILLEGAL_OP;
    }
    auto container = containerOptional.value();
    if (container.getId() != op.getContainerId()) {
        return CraneOperationResult::FAIL_ILLEGAL_OP;
    }


    // If can't load to container to requested position do nothing and return error
    if (!ship.getCargo().canLoadContainerOnTop(std::get<0>(loadTo), std::get<1>(loadTo))) {
        std::cout << "Error moving container, could not add container on top ("
                  << std::get<0>(loadTo) << ", " << std::get<1>(loadTo) << ")" << std::endl;
        return CraneOperationResult::FAIL_ILLEGAL_OP;
    }

    //If can remove container and add to required position, do the action
    ship.getCargo().removeTopContainer(std::get<0>(unloadFrom), std::get<1>(unloadFrom));
    ship.getCargo().loadContainerOnTop(std::get<0>(loadTo), std::get<1>(loadTo), container);

    return CraneOperationResult::SUCCESS;
}

CraneOperationResult CranesManagement::preformOperation(const PackingOperation &op) {
    switch (op.getType()) {
        case PackingType::load :
            return preformLoadOperation(op, currentPort, ship);
        case PackingType::unload :
            return preformUnloadOperation(op, currentPort, ship);
        case PackingType::move :
            return preformMoveOperation(op,ship);
        default:
            return CraneOperationResult::FAIL_ILLEGAL_OP;
    }
}

// endregion
