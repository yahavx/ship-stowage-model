//
// Created by Orr on 5/6/2020.
//
#include "NaiveStowageAlgorithm.h"
#include <algorithm>
#include "../common/actors/CranesManagement.h"
#include "../common/utils/Constants.h"
#include "../common/io/ObjectsReader.h"
#include "../common/utils/UtilFunctions.h"
#include "RobustStowageAlgorithm.h"


int RobustStowageAlgorithm::getInstructionsForCargo(const std::string &inputFile, const std::string &outputFile) {
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

std::string RobustStowageAlgorithm::getAlgorithmName() {
    return "RobustStowageAlgorithm";
}

Operations RobustStowageAlgorithm::generateOperations(ContainerShip &ship, Port &port, const Containers &containersToLoad) {
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
        Operations loadOps = ship.loadContainerToLowestPositionAvailable(port, container);

        // Add load operations to set of all instructions
        operations.addOperations(loadOps);
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
