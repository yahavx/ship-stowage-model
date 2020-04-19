//
// Created by Orr on 4/16/2020.
//

#include "NaiveStowageAlgorithm.h"
#include "../CranesOperation.h"
#include "../../common/io/ObjectsReader.h"
#include "../../utils/UtilFunctions.h"


void NaiveStowageAlgorithm::setWeightBalanceCalculator(WeightBalanceCalculator &calculator) {
    this->ship.setBalanceCalculator(calculator);
    this->ship.getBalanceCalculator().setPlan(this->ship.getShipPlan());
}

void NaiveStowageAlgorithm::setShipPlanFromPath(
        const std::string &shipPlanPath) {
    auto shipPlan = *readShipPlanFromFile(shipPlanPath);
    this->ship.setShipPlan(shipPlan);
}

void NaiveStowageAlgorithm::setShipRouteFromPath(const std::string &shipRoutePath) {
    auto route = *readShipRouteFromFile(shipRoutePath);
    this->ship.setShipRoute(route);
}

void NaiveStowageAlgorithm::getInstructionsForCargo(const std::string &inputFile, const std::string &outputFile) {
    Port port;
    initializePort(inputFile, port);

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
}


/// Modifies the port, inits his id and storage if exist.
void NaiveStowageAlgorithm::initializePort(const std::string &inputFile, Port &port) const {
    if (startsWith(inputFile, unloadOnly)) {  // TODO: communicate this unloadOnly in a more proper way
        std::string portCode = inputFile.substr(11, 5);
        port.setId(PortId(portCode));
    } else {
        std::optional<ContainerStorage> storage = readCargoToPortFromFile(inputFile);

        if (!storage.has_value()) {
            std::cout << "Error in getting instructions for cargo: couldn't load port" << std::endl;

        } else {
            port.setStorage(*storage);
        }
    }
}
