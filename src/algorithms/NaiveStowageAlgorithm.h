//
// Created by Orr on 4/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H
#define SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H


#include "../interfaces/AbstractAlgorithm.h"
#include "../common/actors/ContainerShip.h"
#include "SemiAbstractAlgorithm.h"
#include "../interfaces/AlgorithmRegistration.h"

/**
 * A naive implementation.
 */
class NaiveStowageAlgorithm : public SemiAbstractAlgorithm {

private:

    /**
    * Receives the ship, next id of port to dock into, and list of containers that needs to be loaded.
    * @return list of operations: first unload all containers for this port, and than load all the containers that needs to be loaded.
    */
    Operations generateOperations(ContainerShip &ship, Port &port, const Containers &containersToLoad) override;
};


#endif //SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H
