//
// Created by Orr on 5/16/2020.
//

#include "LoadContainerToLowestPositionAvailable.h"
#include "../actors/CranesManagement.h"

Operations LoadContainerToLowestPositionAvailable::loadContainer(Port &port, ContainerShip &ship, const Container &container, Errors &errors) {
    CranesManagement crane(ship, port);
    Operations ops = Operations();
    std::optional<Position> posOptional = findLowestSpot(port, ship, container);

    if (!posOptional.has_value()) { // Couldn't find position to load the container to
        ops = Operations();
        ops.addOperation({PackingType::reject, container.getId()});
        errors.addError({ErrorFlag::ContainersAtPort_ContainersExceedsShipCapacity, container.getId()});
        return ops;
    } else {
        auto op = PackingOperation(PackingType::load, container.getId(), posOptional.value());
        auto result = crane.preformOperation(op);
        if (result == CraneOperationResult::SUCCESS) // Successfully loaded
            ops.addOperation(op);
        else {
            ops = Operations();
            ops.addOperation({PackingType::reject, container.getId()});
            return ops;
        }
    }

    return ops;
}

std::optional<Position> LoadContainerToLowestPositionAvailable::findLowestSpot(Port &port, ContainerShip &ship, const Container &container) {
    CranesManagement crane(ship, port);
    Operations ops = Operations();
    Dimensions dims = ship.getShipPlan().getDimensions();
    int minZ = dims.Z() + 1, minX = -1, minY = -1;
    // Loop over all possible ship matrix cells and try to load the container on top, until success
    for (int x = 0; (x < dims.X()); x++) {
        for (int y = 0; (y < dims.Y()); y++) {
            WeightBalanceCalculator::BalanceStatus status = ship.getBalanceCalculator().tryOperation('L', container.getWeight(), x, y);
            if (status == WeightBalanceCalculator::BalanceStatus::APPROVED) {
                int z = ship.getCargo().getAvailableFloorToLoadContainer(x, y);
                if (z >= 0 && z < minZ) {
                    minZ = z, minX = x, minY = y;
                }
            }
        }
    }

    if (minZ >= dims.Z() + 1)
        return {};
    return Position(minX, minY, minZ);
}


