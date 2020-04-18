//
// Created by Orr on 4/16/2020.
//

#include "NaiveStowageAlgorithm.h"
#include "../CranesOperation.h"
#include "../../common/io/ObjectsReader.h"


void NaiveStowageAlgorithm::setWeightBalanceCalculator(WeightBalanceCalculator &calculator) {
    this->ship.setBalanceCalculator(calculator);
}

void NaiveStowageAlgorithm::getInstructionsForCargo(const std::string &input_full_path_and_file_name,
                                                    const std::string &output_full_path_and_file_name) {
    // TODO: Read input file and initiate port object, line below is a mockup
    PortId currentPortId("test");
    Port port(currentPortId);

    Containers containersToLoad = Containers();
    // Collect all containers that needs to be loaded
    for (const PortId &id : ship.getShipRoute().getPorts()) {
        Containers portContainers = port.getContainersForDestination(id);
        containersToLoad.insert(containersToLoad.end(), containersToLoad.begin(), containersToLoad.end());
    }
    // Get instructions for unloading and loading from ship
    OPS instructions = ship.dock(currentPortId, containersToLoad);

    // Perform operations on local shop and port
    for (const PackingOperation &op : instructions) {
        CranesOperation::preformOperation(op, port, ship);
        // TODO: Handle failing operation, for example maybe return the container to port
    }

    //TODO: Write instructions to output file
}

void NaiveStowageAlgorithm::setShipPlanFromPath(const std::string &shipPlanPath) {
    readShipPlanFromFile(shipPlanPath, shipPlan);
}

void NaiveStowageAlgorithm::setShipRouteFromPath(const std::string &shipRoutePath) {
    readShipRouteFromFile(shipRoutePath,shipRoute);
}


