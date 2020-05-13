//
// Created by Orr on 5/6/2020.
//
#include "NaiveStowageAlgorithm.h"
#include <algorithm>
#include "../common/actors/CranesManagement.h"
#include "../common/io/ObjectsReader.h"
#include "../common/utils/UtilFunctions.h"
#include "RobustStowageAlgorithm.h"


Operations RobustStowageAlgorithm::generateOperations(ContainerShip &ship, Port &port, const Containers &containersToLoad, Errors &errors) {
    Operations operations;

    std::vector<ContainerPosition> containersToUnload = ship.getCargo().getContainersForPort(port.getId());

    // Unload all required containers
    for (const ContainerPosition &containerPos: containersToUnload) {
        // Get instructions for removing the container
        Operations unloadOps = ship.unloadContainer(port, containerPos);

        // Add unload operations to set of all instructions
        operations.addOperations(unloadOps);
    }

    if((int)containersToLoad.size() <= ship.getCargo().numberOfEmptyPositions()) {
        // There is space for all required containers, so load them from furthest port to nearest
        for (const Container &container: containersToLoad) {
            // Get instructions for adding the container
            Operations loadOps = ship.loadContainerToLowestPositionAvailable(port, container, errors);

            // Add load operations to set of all instructions
            operations.addOperations(loadOps);
        }
    } else {
        // There is not enough space for all required containers, so load them from nearest port to furthest
        for(int i = containersToLoad.size()-1; i >= 0; i--) {
            const Container &container = containersToLoad[i];

            // Get instructions for adding the container
            Operations loadOps = ship.loadContainerToLowestPositionAvailable(port, container, errors);

            // Add load operations to set of all instructions
            operations.addOperations(loadOps);
        }
    }

    return operations;
}

Containers RobustStowageAlgorithm::getContainersToLoad(Port &port) {
    Containers containersToLoad;
    std::vector<PortId> alreadyCollected;  // To not collect from same port twice

    // Collect all containers that needs to be loaded from last port this ships visits in this travel to first
    for (longUInt i = 0; i < ship.getShipRoute().getPorts().size(); i++) {
        const PortId &id = ship.getShipRoute().getPorts()[ship.getShipRoute().getPorts().size()-1-i];
        auto it = std::find(alreadyCollected.begin(), alreadyCollected.end(), id);
        if (it == alreadyCollected.end()) {  // Not seen yet
            alreadyCollected.push_back(id);
            Containers portContainers = port.getContainersForDestination(id);
            containersToLoad.insert(containersToLoad.end(), portContainers.begin(), portContainers.end());
        }
    }

    return containersToLoad;
}
