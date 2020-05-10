//
// Created by t-yabeny on 5/2/2020.
//

#ifndef SHIP_STOWAGE_MODEL_BADALGORITHM_H
#define SHIP_STOWAGE_MODEL_BADALGORITHM_H

#include "SemiAbstractAlgorithm.h"


class BadAlgorithm : public SemiAbstractAlgorithm {  // use this to test the simulator

public:

    int readShipPlan(const std::string &shipPlanPath) override;

private:

    Operations generateOperations(ContainerShip &ship, Port &port, const Containers &containersToLoad, Errors &errors) override;

};


#endif //SHIP_STOWAGE_MODEL_BADALGORITHM_H
