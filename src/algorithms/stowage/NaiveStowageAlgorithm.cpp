//
// Created by Orr on 4/16/2020.
//

#include "NaiveStowageAlgorithm.h"
#include "../../io/ObjectsReader.h"

void NaiveStowageAlgorithm::readShipPlan(const std::string &filePath) {
    ShipPlan shipPlan;
    bool result = readShipPlanFromFile(filePath, shipPlan);
    if (!result) {
        // Handle error
    }
}

void NaiveStowageAlgorithm::readShipRoute(const std::string &filePath) {
    ShipRoute shipRoute;
    bool result = readShipRouteFromFile(filePath, shipRoute);
    if (!result) {
        // Handle error
    }
}

void NaiveStowageAlgorithm::setWeightBalanceCalculator(WeightBalanceCalculator &calculator) {
    this->ship.setBalanceCalculator(calculator);
}

void NaiveStowageAlgorithm::getInstructionsForCargo(const std::string &inputFileName,
                                                    const std::string &outputFileName) {

    std::vector<PackingOperation> instructions = std::vector<PackingOperation>();

    //TODO: Read input file and initiate port object, line below is a mockup
    PortId currentPortId("test");
    Port port(currentPortId);

    for (PortId id : ship.getShipRoute().getPorts()) {
        std::vector<PackingOperation> newInstructions = ship.dock(id, port.getContainersForDestination(id));
        instructions.insert(instructions.end(), newInstructions.begin(), newInstructions.end());
        //TODO: perform instructions on local copy of ship, so it is updated for the next port in the list
    }


    //TODO: Write instructions to output file
}


