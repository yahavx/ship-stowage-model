//
// Created by Orr on 4/16/2020.
//

#include "NaiveStowageAlgorithm.h"
#include <algorithm>
#include "../CranesOperation.h"
#include "../../common/utils/Constants.h"
#include "../../common/io/ObjectsReader.h"
#include "../../common/utils/UtilFunctions.h"

// region Initialization

std::string NaiveStowageAlgorithm::getAlgorithmName() {
    return "NaiveStowageAlgorithm";
}
// endregion

// region Functions

int NaiveStowageAlgorithm::getInstructionsForCargo(const std::string &inputFile, const std::string &outputFile) {
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

    // Get ops for unloading and loading from ship
    Operations ops = ship.dock(port, containersToLoad);

    writePackingOperationsToFile(outputFile, ops);

    ship.markCurrentVisitDone();  // pop the current port from the ShipRoute

    return errors.toErrorFlag();  // TODO: collect all errors
}
// endregion
