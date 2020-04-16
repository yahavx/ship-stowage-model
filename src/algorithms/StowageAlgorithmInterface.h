//
// Created by Orr on 4/9/2020.
//

#ifndef SHIP_STOWAGE_MODEL_STOWAGEALGORITHMINTERFACE_H
#define SHIP_STOWAGE_MODEL_STOWAGEALGORITHMINTERFACE_H


#include <map>
#include "../actors/ContainerShip.h"

/**
 * Algorithm that for given
 */
class StowageAlgorithmInterface {
private:
    ContainerShip &ship;
    std::map<PortId, Port> &ports;

public:
    StowageAlgorithmInterface(ContainerShip &ship, std::map<PortId, Port> &ports);

    virtual void getInstructionsForCargo(const std::string& input_full_path_and_file_name,
                                         const std::string& output_full_path_and_file_name) = 0;

};


#endif //SHIP_STOWAGE_MODEL_STOWAGEALGORITHMINTERFACE_H
