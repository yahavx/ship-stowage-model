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

    Containers containersToLoad;
    std::vector<PortId> alreadyCollected;  // To not collect from same port twice

    // Collect all containers that needs to be loaded
    for (longUInt i = 1; i < ship.getShipRoute().getPorts().size(); i++) {
        const PortId &id = ship.getShipRoute().getPorts()[i];
        auto it = std::find(alreadyCollected.begin(), alreadyCollected.end(), id);
        if (it == alreadyCollected.end()) {
            alreadyCollected.push_back(id);
            Containers portContainers = port.getContainersForDestination(id);
            containersToLoad.insert(containersToLoad.end(), portContainers.begin(), portContainers.end());
        }
    }

    // Get ops for unloading and loading from ship
    ops.addOperations(ship.dock(port, containersToLoad));

    writePackingOperationsToFile(outputFile, ops);

    ship.advanceToNextPort();  // pop the current port from the ShipRoute

    return errors.toErrorFlag();  // TODO: collect all errors
}
// endregion
