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
    this->ship.setShipRoute(*readShipRouteFromFile(shipRoutePath));
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
    for (const PortId &id : ship.getShipRoute().getPorts()) {
        Containers portContainers = port.getContainersForDestination(id);
        containersToLoad.insert(containersToLoad.end(), portContainers.begin(), portContainers.end());
    }

    ContainerShip tmpShip = ship;
    // Get ops for unloading and loading from ship
    OPS ops = tmpShip.dock(port, containersToLoad);

//    // Perform operations on local ship and port
//    for (const PackingOperation &op : ops) {
//        auto opResult = CranesOperation::preformOperation(op, port, ship);
//        if (opResult == CraneOperationResult::FAIL_CONTAINER_NOT_FOUND)
//            std::cout << "Crane received illegal operation, didn't find container with ID:" << op.getContainerId()
//                      << std::endl;
//        if (opResult == CraneOperationResult::FAIL_ILLEGAL_OP)
//            std::cout << "Crane received illegal operation: " << op << "\n";
//        // TODO: Handle failing operation ??
//    }

    writePackingOperationsToFile(outputFile, ops);
}
