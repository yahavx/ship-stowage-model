//
// Created by Orr on 6/10/2020.
//

#include <bits/unique_ptr.h>
#include "EfficientStowageAlgorithm.h"
#include "../common/strategies/LoadContainerToLowestPositionAvailable.h"
#include "../common/strategies/LoadContainerInGroups.h"

Operations EfficientStowageAlgorithm::generateOperations(ContainerShip &ship, Port &port, const Containers &containersToLoad, Errors &errors) {
    Operations operations;

    std::vector<ContainerPosition> containersToUnload = ship.getCargo().getContainersForPort(port.getId());

    Containers containersToLoadBack;
    std::vector<ContainerPosition> unloadedContainers;

    // Unload all required containers
    for (const ContainerPosition &containerPos: containersToUnload) {

        //If the container was already unloaded, skip it
        if (find(unloadedContainers.begin(), unloadedContainers.end(), containerPos) != unloadedContainers.end())
            continue;

        // Get instructions for removing the container
        Operations unloadOps = unloadContainer(ship, port, containerPos, containersToUnload, unloadedContainers, containersToLoadBack);

        // Add unload operations to set of all instructions
        operations.addOperations(unloadOps);
    }

    std::unique_ptr<LoadContainerStrategy> strategy = std::make_unique<LoadContainerInGroups>();

    for (const Container &container: containersToLoadBack) {
        // Get instructions for adding the container
        Operations loadOps = ship.loadContainer(strategy.get(), port, container, errors);

        // Add load operations to set of all instructions
        operations.addOperations(loadOps);
    }

    if ((int) containersToLoad.size() <= ship.getCargo().numberOfEmptyPositions()) {
        // There is space for all required containers, so load them from furthest port to nearest
        for (const Container &container: containersToLoad) {
            // Get instructions for adding the container
            Operations loadOps = ship.loadContainer(strategy.get(), port, container, errors);

            // Add load operations to set of all instructions
            operations.addOperations(loadOps);
        }
    } else {
        // There is not enough space for all required containers, so load them from nearest port to furthest
        for (int i = containersToLoad.size() - 1; i >= 0; i--) {
            const Container &container = containersToLoad[i];

            // Get instructions for adding the container
            Operations loadOps = ship.loadContainer(strategy.get(), port, container, errors);

            // Add load operations to set of all instructions
            operations.addOperations(loadOps);
        }
    }

    return operations;
}

Containers EfficientStowageAlgorithm::getContainersToLoad(Port &port) {
    Containers containersToLoad;
    std::vector<PortId> alreadyCollected;  // To not collect from same port twice

    // Collect all containers that needs to be loaded from last port this ships visits in this travel to first
    for (longUInt i = 0; i < ship.getShipRoute().getPorts().size(); i++) {
        const PortId &id = ship.getShipRoute().getPorts()[ship.getShipRoute().getPorts().size() - 1 - i];
        auto it = std::find(alreadyCollected.begin(), alreadyCollected.end(), id);
        if (it == alreadyCollected.end()) {  // Not seen yet
            alreadyCollected.push_back(id);
            Containers portContainers = port.getContainersForDestination(id);
            containersToLoad.insert(containersToLoad.end(), portContainers.begin(), portContainers.end());
        }
    }

    return containersToLoad;
}

Operations
EfficientStowageAlgorithm::unloadContainer(ContainerShip &ship, Port &port, const ContainerPosition &containerPos,
                                           std::vector<ContainerPosition> containersToUnload,
                                           std::vector<ContainerPosition> unloadedContainers,
                                           Containers &containersToLoadBack) {

    int x = containerPos.x(), y = containerPos.y();
    Operations ops;
    int currentHeight = ship.getCargo().currentTopHeight(x, y) - 1;
    while (currentHeight > containerPos.z()) {
        Container topContainer = ship.getCargo().getTopContainer(x, y).value();
        ContainerPosition topContainerPos(topContainer, Position(x, y, currentHeight));
        auto topContainerIterator = std::find(containersToUnload.begin(), containersToUnload.end(), topContainerPos);

        // If there are container on top that should not be unloaded try moving it
        if (topContainerIterator == containersToUnload.end()) {
            // Try moving the container
            auto moveOps = tryMovingContainer(ship, port, topContainerPos, containersToUnload);

            if (!moveOps.has_value()) { // If failed to move the container unload it temporarily
                ops.addOperations(ship.unloadContainer(port, topContainerPos));
                containersToLoadBack.push_back(topContainer);
            } else {
                ops.addOperations(*moveOps);
            }
        } else { // Else just unload the container
            ops.addOperations(ship.unloadContainer(port, *topContainerIterator));
            unloadedContainers.push_back(*topContainerIterator);
        }

        // Update the current height of x,y
        currentHeight = ship.getCargo().currentTopHeight(containerPos.x(), containerPos.y()) - 1;
    }

    auto topContainerIterator = std::find(containersToUnload.begin(), containersToUnload.end(), containerPos);
    ops.addOperations(ship.unloadContainer(port, containerPos));
    unloadedContainers.push_back(*topContainerIterator);

    return ops;
}

std::optional<Operations>
EfficientStowageAlgorithm::tryMovingContainer(ContainerShip &ship, Port &port, ContainerPosition &pos, std::vector<ContainerPosition> &containersToUnload) {
    auto posToMoveTo = findPosToMoveTo(ship, pos, containersToUnload);
    if (!posToMoveTo.has_value())
        return {};

    CranesManagement crane(ship, port);
    PackingOperation op(PackingType::move, pos.getContainer().getId(), pos.getPosition(), *posToMoveTo);
    auto result = crane.preformOperation(op);
    if (result == CraneOperationResult::SUCCESS) { // Successfully loaded
        Operations ops;
        ops.addOperation(op);
        return ops;
    }

    return {};

}

std::optional<Position>
EfficientStowageAlgorithm::findPosToMoveTo(ContainerShip &ship, ContainerPosition &pos, std::vector<ContainerPosition> &containersToUnload) {

    Dimensions dims = ship.getShipPlan().getDimensions();
    int z, foundX = -1, foundY = -1, foundZ = -1;
    for (int x = 0; x < dims.X(); x++)
        for (int y = 0; y < dims.Y(); y++) {
            z = ship.getCargo().getAvailableFloorToLoadContainer(x, y);

            if (z >= 0) { // There is space to load on top in this x,y position
                bool foundContainerToUnload = false;
                // Check if there is container to be unloaded in this x,y
                for (auto &containerPos : containersToUnload) {
                    if (ship.getCargo().posHasContainer(x, y, containerPos.getContainer())) {
                        foundContainerToUnload = true;
                        break;
                    }
                }

                if (!foundContainerToUnload) { // There is no container to unload in this x,y position so return it
                    foundX = x;
                    foundY = y;
                    foundZ = ship.getCargo().getAvailableFloorToLoadContainer(x, y);
                    auto topContainerOptional = ship.getCargo().getTopContainer(x, y);
                    if (topContainerOptional.has_value()) {
                        // If found position to move to that the top container has the same destination port as the required container return it
                        if (topContainerOptional->getDestPort() == pos.getContainer().getDestPort())
                            return Position(foundX, foundY, foundZ);
                    }
                }
            }
        }

    // If found some position to move to return it. note that this is the second best option because it means we couldn't find
    // position with the top container has the same destination port as the required container
    if (foundX >= 0)
        return Position(foundX, foundY, foundZ);

    return {};
}
