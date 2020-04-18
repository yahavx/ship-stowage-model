//
// Created by Orr on 4/16/2020.
//

#include "NaiveStowageAlgorithm.h"
#include "../CranesOperation.h"
#include "../../common/io/ObjectsReader.h"


void NaiveStowageAlgorithm::setWeightBalanceCalculator(WeightBalanceCalculator &calculator) {
    this->ship.setBalanceCalculator(calculator);
}

void NaiveStowageAlgorithm::getInstructionsForCargo(const std::string &inputFile, const std::string &outputFile) {
    // TODO: Read input file and initiate port object, line below is a mockup
    PortId currentPortId("test");
    Port port(currentPortId);

    Containers containersToLoad = Containers();
    // Collect all containers that needs to be loaded
    for (const PortId &id : ship.getShipRoute().getPorts()) {
        Containers portContainers = port.getContainersForDestination(id);
        containersToLoad.insert(containersToLoad.end(), containersToLoad.begin(), containersToLoad.end());
    }
    // Get ops for unloading and loading from ship
    OPS ops = ship.dock(currentPortId, containersToLoad);

    // Perform operations on local shop and port
    for (const PackingOperation &op : ops) {
        auto opResult = CranesOperation::preformOperation(op, port, ship);
        if(opResult == CraneOperationResult::FAIL_CONTAINER_NOT_FOUND)
            std::cout << "Crane got illegal operation, didn't find container with ID:" << op.getContainerId() << "\n";
        if(opResult == CraneOperationResult::FAIL_ILLEGAL_OP)
            std::cout << "Crane got illegal operation" << op << "\n";
        // TODO: Handle failing operation ??
    }

    writePackingOperationsToFile(outputFile, ops);
}

void NaiveStowageAlgorithm::setShipPlanFromPath(const std::string &shipPlanPath) {
    readShipPlanFromFile(shipPlanPath, shipPlan);
}

void NaiveStowageAlgorithm::setShipRouteFromPath(const std::string &shipRoutePath) {
    readShipRouteFromFile(shipRoutePath, shipRoute);
}


