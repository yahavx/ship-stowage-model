//
// Created by t-yabeny on 5/2/2020.
//

#include "BadAlgorithm.h"
#include "../common/io/ObjectsReader.h"

// region Initialization

std::string BadAlgorithm::getAlgorithmName() {
    return "BadAlgorithm";
}
// endregion

int BadAlgorithm::getInstructionsForCargo(const std::string &inputFile, const std::string &outputFile) {
    if (this->algoErrors) {
        return this->algoErrors;
    }

    Errors errors;

    PortId id = this->ship.getShipRoute().getFirstPort();
    ContainerStorage storage = readPortCargoFromFile(inputFile, errors);
    Port port(id, storage);

    Containers containersToLoad;

    std::vector<PortId> ids;

    // Collect all containers that needs to be loaded
    for (longUInt i = 1; i < this->ship.getShipRoute().getPorts().size(); i++) {
        const PortId &id = ship.getShipRoute().getPorts()[i];
        auto it = std::find(ids.begin(), ids.end(), id);
        if (it != ids.end())
            continue;
        ids.push_back(id);
        Containers portContainers = port.getContainersForDestination(id);
        containersToLoad.insert(containersToLoad.end(), portContainers.begin(), portContainers.end());
    }

//    Operations ops = ship.dock(port, containersToLoad);
    Operations ops = Operations();  // intentional bug

    writePackingOperationsToFile(outputFile, ops);

    ship.markCurrentVisitDone(); // pop the current port from the ShipRoute

    return errors.toErrorFlag();
}
