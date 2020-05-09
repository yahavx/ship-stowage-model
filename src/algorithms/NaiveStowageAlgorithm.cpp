//
// Created by Orr on 4/16/2020.
//

#include "NaiveStowageAlgorithm.h"
#include <algorithm>
#include "../common/actors/CranesManagement.h"
#include "../common/utils/Constants.h"
#include "../common/io/ObjectsReader.h"
#include "../common/utils/UtilFunctions.h"
#include <unordered_set>

#ifdef RUNNING_ON_NOVA
REGISTER_ALGORITHM(NaiveStowageAlgorithm)
#endif

// region Functions

Operations NaiveStowageAlgorithm::generateOperations(ContainerShip &ship, Port &port, const Containers &containersToLoad) {
    Operations operations;

    std::vector<ContainerPosition> containersToUnload = ship.getCargo().getContainersForPort(port.getId());

    // Unload all required containers
    for (const ContainerPosition &containerPos: containersToUnload) {
        // Get instructions for removing the container
        Operations unloadOps = ship.unloadContainer(port, containerPos);
        // Add unload operations to set of all instructions
        operations.addOperations(unloadOps);
    }

    // Load all required containers
    for (const Container &container: containersToLoad) {
        // Get instructions for adding the container
        Operations loadOps = ship.loadContainerToArbitraryPosition(port, container);

        // Add load operations to set of all instructions
        operations.addOperations(loadOps);
    }

    return operations;
}

// endregion
