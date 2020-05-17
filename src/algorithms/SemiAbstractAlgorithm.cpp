//
// Created by t-yabeny on 5/2/2020.
//

#include "SemiAbstractAlgorithm.h"
#include "../common/utils/Errors.h"
#include "../common/io/ObjectsReader.h"
#include "../common/utils/UtilFunctions.h"


// region Init

int SemiAbstractAlgorithm::readShipPlan(const std::string &shipPlanPath) {
    Errors errors;
    auto shipPlan = readShipPlanFromFile(shipPlanPath, errors);
    this->ship.setShipPlan(shipPlan);

    if (errors.hasFatalError()) {
        this->algoErrors = errors.toErrorFlag();
    } else {
        this->algoErrors &= (~ErrorFlag::ShipPlan_FatalError_NoFileOrInvalidFirstLine);  // turn off the flag
    }

    return errors.toErrorFlag();
}

int SemiAbstractAlgorithm::readShipRoute(const std::string &shipRoutePath) {
    Errors errors;
    ShipRoute route = readShipRouteFromFile(shipRoutePath, errors);
    this->ship.setShipRoute(route);

    if (errors.hasFatalError()) {
        this->algoErrors = errors.toErrorFlag();
    } else {
        this->algoErrors &= (~ErrorFlag::ShipRoute_FatalError_NoFileOrNoLegalPorts);  // turn off the flag
    }

    return errors.toErrorFlag();
}

int SemiAbstractAlgorithm::setWeightBalanceCalculator(WeightBalanceCalculator &calculator) {
    this->ship.setBalanceCalculator(calculator);
    return 0;
}

// endregion

// region Functions

int SemiAbstractAlgorithm::getInstructionsForCargo(const std::string &inputFile, const std::string &outputFile) {
    if (hasFatalError()) {  // Not initialized, or bad plan/route
        return algoErrors;
    }

    Operations ops;
    Errors errors;

    Port port(ship.getShipRoute().getFirstPort());
    ContainerStorage storage = readPortCargoFromFile(inputFile, errors);
    bool isLastPort = ship.getShipRoute().isLastPort() ;

    if (isLastPort && !storage.isEmpty()) {
        errors.addError({ErrorFlag::ContainersAtPort_LastPortHasContainers});
    }

    if (!isLastPort) {  // we ignore storage in the last port
        port.setStorage(std::move(storage));
    }

    Containers containersToLoad;

    if (!isLastPort) {
        StringVector toReject = port.removeBadContainers(ship.getShipRoute(), errors);  // Get ids of all rejected containers (can contain duplicates)
        ops.addRejectOperations(toReject);  // If its empty, nothing will be added

        if (!port.getStorage().isEmpty()) {
            containersToLoad = getContainersToLoad(port);

            // Filter containers and keep only valid ones
            toReject = ship.filterContainers(containersToLoad, errors);
            ops.addRejectOperations(toReject);  // If its empty, nothing will be added
        }
    }

    ops.addOperations(this->generateOperations(ship, port, containersToLoad, errors));  // Get ops for unloading and loading from ship

    writePackingOperationsToFile(outputFile, ops);

    ship.advanceToNextPort();  // pop the current port from the ShipRoute

    return errors.toErrorFlag();
}

Containers SemiAbstractAlgorithm::getContainersToLoad(Port &port) {
    Containers containersToLoad;
    std::vector<PortId> alreadyCollected;  // To not collect from same port twice

    // Collect all containers that needs to be loaded
    for (longUInt i = 1; i < ship.getShipRoute().getPorts().size(); i++) {
        const PortId &currentPort = ship.getShipRoute().getPorts()[i];
        bool found = false;

        for (auto &portSeen: alreadyCollected) {
            if (portSeen == currentPort) {
                found = true;
                break;
            }
        }

        if (!found) {
            alreadyCollected.push_back(currentPort);
            Containers portContainers = port.getContainersForDestination(currentPort);
            containersToLoad.insert(containersToLoad.end(), portContainers.begin(), portContainers.end());
        }
    }

    return containersToLoad;
}

bool SemiAbstractAlgorithm::hasFatalError() {
    return algoErrors;
}

// endregion


