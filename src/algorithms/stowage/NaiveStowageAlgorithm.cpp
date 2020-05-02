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


/// Returns true if inputFile is not a file (unloadOnly).  // TODO: remove this (we don't get s_unloadOnly anymore)
bool NaiveStowageAlgorithm::initPortId(const std::string &inputFile, Port &port) const {
    if (startsWith(inputFile, Constants::s_unloadOnly)) {
        std::string portCode = inputFile.substr(11, 5);
        port.setId(PortId(portCode));
        return true;
    } else {
        std::string id = extractFilenameFromPath(inputFile, true).substr(0, 5);  // extract port id from path
        port.setId(PortId(id));
        return false;
    }
}

/// Modifies the port, inits his id and storage if exist.
void NaiveStowageAlgorithm::initializePort(const std::string &inputFile, Port &port) const {
    bool isUnloadOnly = initPortId(inputFile, port);

    if (isUnloadOnly) {
        return;
    }

    std::vector<ErrorFlag> cargoErrors;
    ContainerStorage storage = readPortCargoFromFile(inputFile, cargoErrors);

    port.setStorage(storage);
}
// endregion

// region Functions

int NaiveStowageAlgorithm::getInstructionsForCargo(const std::string &inputFile, const std::string &outputFile) {
    if (this->fatalError) {
        return this->fatalError;
    }

    Port port;
    initializePort(inputFile, port);
    int errorsFlag = 0;

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
    OPS ops = ship.dock(port, containersToLoad);

    writePackingOperationsToFile(outputFile, ops);

    ship.markCurrentVisitDone(); // pop the current port from the ShipRoute

    return errorsFlag;  // TODO: collect all errors
}
// endregion
