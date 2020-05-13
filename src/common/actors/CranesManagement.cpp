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
    const Position &pos = op.getFirstPosition();
    int result = ship.getCargo().loadContainerOnTop(pos.X(), pos.Y(), container);

    // If load to ship failed, put container back at port and return false
    if (result < 0) {
        port.addContainer(container);
        return CraneOperationResult::FAIL_ILLEGAL_OP;
    }

    return CraneOperationResult::SUCCESS;
}

CraneOperationResult preformUnloadOperation(const PackingOperation &op, Port &port, ContainerShip &ship) {
    const Position &pos = op.getFirstPosition();
    auto containerOptional = ship.getCargo().getTopContainer(pos.X(), pos.Y());
    if (!containerOptional.has_value()) {
        std::cout << "Error unloading container, could not remove top container from cargo ("
                  << pos.X() << ", " << pos.Y() << ")" << std::endl;
        return CraneOperationResult::FAIL_ILLEGAL_OP;
    }
    auto container = containerOptional.value();
    if (container.getId() != op.getContainerId()) {
        return CraneOperationResult::FAIL_ILLEGAL_OP;
    }

    ship.getCargo().removeTopContainer(pos.X(), pos.Y());
    port.addContainer(container);

    return CraneOperationResult::SUCCESS;
}

CraneOperationResult preformMoveOperation(const PackingOperation &op, ContainerShip &ship) {
    const Position &unloadFrom = op.getFirstPosition();
    const Position &loadTo = op.getSecondPosition();
    auto containerOptional = ship.getCargo().getTopContainer(unloadFrom.X(), unloadFrom.Y());

    // If can't remove container do nothing and return error
    if (!containerOptional.has_value()) {
        std::cout << "Error moving container, could not remove top container from cargo ("
                  << unloadFrom.X() << ", " << unloadFrom.Y() << ")" << std::endl;
        return CraneOperationResult::FAIL_ILLEGAL_OP;
    }
    auto container = containerOptional.value();
    if (container.getId() != op.getContainerId()) {
        return CraneOperationResult::FAIL_ILLEGAL_OP;
    }

    // If can't load to container to requested position do nothing and return error
    if (!ship.getCargo().getAvailableFloorToLoadContainer(loadTo.X(), loadTo.Y())) {
        std::cout << "Error moving container, could not add container on top ("
                  << loadTo.X() << ", " << loadTo.Y() << ")" << std::endl;
        return CraneOperationResult::FAIL_ILLEGAL_OP;
    }

    //If can remove container and add to required position, do the action
    ship.getCargo().removeTopContainer(unloadFrom.X(), unloadFrom.Y());
    ship.getCargo().loadContainerOnTop(loadTo.X(), loadTo.Y(), container);

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
