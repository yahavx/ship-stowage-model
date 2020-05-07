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
//    this->ship.getBalanceCalculator().setPlan(this->ship.getShipPlan());  // TODO: find another way to set ship plan, its not part of the interface
    return 0;
}

// endregion

// region Functions

Containers SemiAbstractAlgorithm::getContainersToLoad(Port &port) {
    Containers containersToLoad;
    std::vector<PortId> alreadyCollected;  // To not collect from same port twice

    // Collect all containers that needs to be loaded
    for (longUInt i = 1; i < ship.getShipRoute().getPorts().size(); i++) {
        const PortId &id = ship.getShipRoute().getPorts()[i];
        auto it = std::find(alreadyCollected.begin(), alreadyCollected.end(), id);
        if (it == alreadyCollected.end()) {  // Not seen yet
            alreadyCollected.push_back(id);
            Containers portContainers = port.getContainersForDestination(id);
            containersToLoad.insert(containersToLoad.end(), portContainers.begin(), portContainers.end());
        }
    }

    return containersToLoad;
}

bool SemiAbstractAlgorithm::hasFatalError() {
    return algoErrors;
}

int SemiAbstractAlgorithm::getInstructionsForCargo(const std::string &inputFile, const std::string &outputFile) {
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

    if (ship.getShipRoute().isLastPort() && !containersToLoad.empty()) {
        errors.addError({ErrorFlag::ContainersAtPort_LastPortHasContainers});
        containersToLoad = Containers();
    }

    ops.addOperations(this->generateOperations(ship, port, containersToLoad));  // Get ops for unloading and loading from ship

    writePackingOperationsToFile(outputFile, ops);

    ship.advanceToNextPort();  // pop the current port from the ShipRoute

    return errors.toErrorFlag();
}

// endregion


