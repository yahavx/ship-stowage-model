//
// Created by Orr on 5/16/2020.
//

#include "LoadContainerToArbitraryPosition.h"
#include "../actors/CranesManagement.h"

Operations LoadContainerToArbitraryPosition::loadContainer(Port &port, ContainerShip &ship, const Container &container, Errors &errors) {
    std::optional<Operations> opsOptional = findFreeSpotToLoadTo(port, ship, container);

    if (!opsOptional.has_value()) {
        Operations ops = Operations();
        ops.addOperation({PackingType::reject, container.getId()});
        errors.addError({ErrorFlag::ContainersAtPort_ContainersExceedsShipCapacity, container.getId()});
        return ops;
    }

    return opsOptional.value();
}

std::optional<Operations> LoadContainerToArbitraryPosition::findFreeSpotToLoadTo(Port &port, ContainerShip &ship, const Container &container) {
    CranesManagement crane(ship, port);
    Operations ops = Operations();
    Dimensions dims = ship.getShipPlan().getDimensions();
    int z = -1;
    for (int x = 0; x < dims.X(); x++)
        for (int y = 0; y < dims.Y(); y++) {
            WeightBalanceCalculator::BalanceStatus status = ship.getBalanceCalculator().tryOperation('L', container.getWeight(), x, y);
            if (status == WeightBalanceCalculator::BalanceStatus::APPROVED) {
                z = ship.getCargo().getAvailableFloorToLoadContainer(x, y);
                if (z >= 0) {
                    std::optional<Operations> opsOptional = createOperations(container, crane, {x, y, z});
                    if (opsOptional.has_value())
                        return opsOptional.value();
                }
            }
        }

    return {};
}

std::optional<Operations> LoadContainerToArbitraryPosition::createOperations(const Container &container, CranesManagement &crane, Position pos) {
    auto op = PackingOperation(PackingType::load, container.getId(), pos);
    auto result = crane.preformOperation(op);
    if (result == CraneOperationResult::SUCCESS) { /// Successfully loaded
        Operations ops;
        ops.addOperation(op);
        return ops;
    }

    return {};
}

