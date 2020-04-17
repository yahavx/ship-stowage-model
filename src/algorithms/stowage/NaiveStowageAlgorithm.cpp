//
// Created by Orr on 4/16/2020.
//

#include "NaiveStowageAlgorithm.h"
#include "../CranesOperation.h"

void NaiveStowageAlgorithm::readShipPlan(const std::string &full_path_and_file_name) {
    //TODO: read ship plan from file and load into ship object
}

void NaiveStowageAlgorithm::readShipRoute(const std::string &full_path_and_file_name) {
    //TODO: read ship route from file and load into ship object
}

void NaiveStowageAlgorithm::setWeightBalanceCalculator(WeightBalanceCalculator &calculator) {
    this->ship.setBalanceCalculator(calculator);
}

void NaiveStowageAlgorithm::getInstructionsForCargo(const std::string &input_full_path_and_file_name,
                                                    const std::string &output_full_path_and_file_name) {
    //TODO: Read input file and initiate port object, line below is a mockup
    PortId currentPortId("test");
    Port port(currentPortId);

    std::vector<Container> containersToLoad = std::vector<Container>();
    //Collect all containers that needs to be loaded
    for (const PortId &id : ship.getShipRoute().getPorts()) {
        std::vector<Container> portContainers = port.getContainersForDestination(id);
        containersToLoad.insert(containersToLoad.end(), containersToLoad.begin(), containersToLoad.end());
    }
    //Get instructions for unloading and loading from ship
    std::vector<PackingOperation> instructions = ship.dock(currentPortId, containersToLoad);

    //Perform operations on local shop and port
    for (const PackingOperation &op : instructions) {
        CranesOperation::preformOperation(op, port, ship);
        //TODO: Handle failing operation, for example maybe return the container to port
    }

    //TODO: Write instructions to output file
}


