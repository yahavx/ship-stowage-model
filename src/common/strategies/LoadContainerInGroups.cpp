//
// Created by Orr on 6/11/2020.
//

#include <bits/unique_ptr.h>
#include "LoadContainerInGroups.h"
#include "../actors/CranesManagement.h"
#include "LoadContainerToLowestPositionAvailable.h"

Operations LoadContainerInGroups::loadContainer(Port &port, ContainerShip &ship, const Container &container, Errors &errors) {
    CranesManagement crane(ship, port);
    Operations ops = Operations();
    std::optional<Position> posOptional = findLowestSpotWithSameDestination(ship, container);

    if (!posOptional.has_value()) { // Couldn't find position to load the container to, use simpler strategy
        std::unique_ptr<LoadContainerStrategy> strategyLowest = std::make_unique<LoadContainerToLowestPositionAvailable>();
        return strategyLowest->loadContainer(port, ship, container, errors);
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

std::optional<Position> LoadContainerInGroups::findLowestSpotWithSameDestination(ContainerShip &ship, const Container &container) {
    Operations ops = Operations();
    Dimensions dims = ship.getShipPlan().getDimensions();
    int minZ = dims.Z() + 1, minX = -1, minY = -1;
    // Loop over all possible ship matrix cells and try to load the container on top, until success
    for (int x = 0; (x < dims.X()); x++) {
        for (int y = 0; (y < dims.Y()); y++) {
            WeightBalanceCalculator::BalanceStatus status = ship.getBalanceCalculator().tryOperation('L', container.getWeight(), x, y);
            if (status == WeightBalanceCalculator::BalanceStatus::APPROVED) {
                int z = ship.getCargo().getAvailableFloorToLoadContainer(x, y);
                auto topContainer = ship.getCargo().getTopContainer(x, y);

                // If the top container in this position has destination as the given container destination, and it is the lowest position found yet
                if (topContainer.has_value() && topContainer.value().getDestPort() == container.getDestPort() && z >= 0 && z < minZ) {
                    minZ = z, minX = x, minY = y;
                }
            }
        }
    }

    if (minZ >= dims.Z() + 1)
        return {};
    return Position(minX, minY, minZ);
}