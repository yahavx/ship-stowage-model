//
// Created by Orr on 4/16/2020.
//

#include "NaiveStowageAlgorithm.h"
#include "../CranesOperation.h"
#include "../../io/ObjectsReader.h"

ShipPlan NaiveStowageAlgorithm::readShipPlan(const std::string &filePath) {
    ShipPlan shipPlan;
    bool result = readShipPlanFromFile(filePath, shipPlan);
    if (!result) {
        // TODO: handle error
    }
    return shipPlan;
}

ShipRoute NaiveStowageAlgorithm::readShipRoute(const std::string &filePath) {
    ShipRoute shipRoute;
    bool result = readShipRouteFromFile(filePath, shipRoute);
    if (!result) {
        // TODO: handle error
    }
    return shipRoute;
}

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

