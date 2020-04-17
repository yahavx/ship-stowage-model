//
// Created by Orr on 4/16/2020.
//

#include "NaiveStowageAlgorithm.h"

void NaiveStowageAlgorithm::readShipPlan(const std::string &full_path_and_file_name) {
    //TODO: read ship plan from file and load into ship object
}

void NaiveStowageAlgorithm::readShipRoute(const std::string &full_path_and_file_name) {
    //TODO: read ship route from file and load into ship object
}

void NaiveStowageAlgorithm::setWeightBalanceCalculator(WeightBalanceCalculator &calculator) {
    this->ship.setBalanceCalculator(calculator);
}

void NaiveStowageAlgorithm::getInstructionsForCargo(const std::string& input_full_path_and_file_name,
                                                    const std::string& output_full_path_and_file_name) {

    std::vector<PackingOperation> instructions = std::vector<PackingOperation>();

    //TODO: Read input file and initiate port object, line below is a mockup
    PortId currentPortId("test");
    Port port(currentPortId);

    for(PortId id : ship.getShipRoute().getPorts()) {
        std::vector<PackingOperation> newInstructions = ship.dock(id, port.getContainersForDestination(id));
        instructions.insert(instructions.end(), newInstructions.begin(), newInstructions.end());
        //TODO: perform instructions on local copy of ship, so it is updated for the next port in the list
    }


    //TODO: Write instructions to output file
}


