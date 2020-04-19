//
// Created by Orr on 4/16/2020.
//

#include "NaiveStowageAlgorithm.h"
#include "../CranesOperation.h"
#include "../../common/io/ObjectsReader.h"
#include "../../utils/UtilFunctions.h"


void NaiveStowageAlgorithm::setWeightBalanceCalculator(WeightBalanceCalculator &calculator) {
    this->ship.setBalanceCalculator(calculator);
}

void NaiveStowageAlgorithm::setShipPlanFromPath(
        const std::string &shipPlanPath) {  // TODO: verify the <optional> is valid? (the simulator will kill the run before this call if invalid)
    auto shipPlan = *readShipPlanFromFile(shipPlanPath);
    this->ship.setShipPlan(shipPlan);
}

void NaiveStowageAlgorithm::setShipRouteFromPath(const std::string &shipRoutePath) {
    auto route = *readShipRouteFromFile(shipRoutePath);
    this->ship.setShipRoute(route);
}

void NaiveStowageAlgorithm::getInstructionsForCargo(const std::string &inputFile, const std::string &outputFile) {
    Port port;
    if (startsWith(inputFile, unloadOnly)) {  // TODO: make this not arabic
        std::string portCode = inputFile.substr(11, 5);
        port.setId(PortId(portCode));
    }

    else {
        std::optional<Port> optPort = readCargoToPortFromFile(inputFile);

        if (!optPort.has_value()) {
            std::cout << "Error in getInstructionsForCargo(): couldn't load port" << std::endl;
        }
        port = *optPort;
    }

    Containers containersToLoad = Containers();
    // Collect all containers that needs to be loaded
    // TODO: only remaining route should be considered
    for (longUInt i = 1; i < this->ship.getShipRoute().getPorts().size(); i++) {
        const PortId &id = ship.getShipRoute().getPorts()[i];
        Containers portContainers = port.getContainersForDestination(id);
        containersToLoad.insert(containersToLoad.end(), portContainers.begin(), portContainers.end());
    }

    // Get ops for unloading and loading from ship
    OPS ops = ship.dock(port, containersToLoad);

    writePackingOperationsToFile(outputFile, ops);

    ship.markCurrentVisitDone();
}
