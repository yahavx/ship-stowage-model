//
// Created by Orr on 4/16/2020.
//

#include "NaiveStowageAlgorithm.h"
#include <algorithm>
#include "../common/actors/CranesOperation.h"
#include "../common/utils/Constants.h"
#include "../common/io/ObjectsReader.h"
#include "../common/utils/UtilFunctions.h"
#include <unordered_set>


// region Initialization

std::string NaiveStowageAlgorithm::getAlgorithmName() {
    return "NaiveStowageAlgorithm";
}
// endregion

// region Functions

int NaiveStowageAlgorithm::getInstructionsForCargo(const std::string &inputFile, const std::string &outputFile) {
    if (hasFatalError()) {  // Not initialized, or bad plan/route
        createEmptyFile(outputFile);
        return algoErrors;
    }

    Operations ops;
    Errors errors;

    PortId id = ship.getShipRoute().getFirstPort();
    ContainerStorage storage = readPortCargoFromFile(inputFile, errors);
    Port port(id, storage);

    StringVector toReject = port.removeBadContainers(ship.getShipRoute());
    ops.addRejectOperations(toReject);  // If its empty, nothing will be added

    Containers containersToLoad = getContainersToLoad(port);
    ops.addOperations(this->generateOperations(ship, port, containersToLoad));  // Get ops for unloading and loading from ship

    writePackingOperationsToFile(outputFile, ops);

    ship.advanceToNextPort();  // pop the current port from the ShipRoute

    return errors.toErrorFlag();
}

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
