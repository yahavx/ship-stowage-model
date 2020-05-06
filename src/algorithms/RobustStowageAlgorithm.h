//
// Created by Orr on 5/6/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ROBUSTSTOWAGEALGORITHM_H
#define SHIP_STOWAGE_MODEL_ROBUSTSTOWAGEALGORITHM_H


#include "SemiAbstractAlgorithm.h"

class RobustStowageAlgorithm : public SemiAbstractAlgorithm {

public:

    int getInstructionsForCargo(const std::string &inputFile, const std::string &outputFile) override;

    std::string getAlgorithmName() override;

private:

    /**
    * Receives the ship, next id of port to dock into, and list of containers that needs to be loaded.
    * @return list of operations: first unload all containers for this port, and than load all the containers that needs to be loaded.
    */
    Operations generateOperations(ContainerShip &ship, Port &port, const Containers &containersToLoad);

    Containers getContainersToLoad(Port &port) override;
};


#endif //SHIP_STOWAGE_MODEL_ROBUSTSTOWAGEALGORITHM_H
