//
// Created by Orr on 4/16/2020.
//

#include "NaiveStowageAlgorithm.h"
#include <algorithm>
#include "../common/actors/CranesManagement.h"
#include "../common/io/ObjectsReader.h"
#include "../common/utils/UtilFunctions.h"
#include "../common/strategies/LoadContainerToArbitraryPosition.h"
#include <unordered_set>


// region Functions

Operations NaiveStowageAlgorithm::generateOperations(ContainerShip &ship, Port &port, const Containers &containersToLoad, Errors &errors) {
    Operations operations;

    std::vector<ContainerPosition> containersToUnload = ship.getCargo().getContainersForPort(port.getId());

    // Unload all required containers
    for (const ContainerPosition &containerPos: containersToUnload) {
        // Get instructions for removing the container
        Operations unloadOps = ship.unloadContainer(port, containerPos);
        // Add unload operations to set of all instructions
        operations.addOperations(unloadOps);
    }

    std::unique_ptr<LoadContainerStrategy> strategy = std::make_unique<LoadContainerToArbitraryPosition>();
    // Load all required containers
    for (const Container &container: containersToLoad) {
        
        // Get instructions for adding the container
        Operations loadOps = ship.loadContainer(strategy.get(), port, container, errors);

        // Add load operations to set of all instructions
        operations.addOperations(loadOps);
    }

    return operations;
}

// endregion
