//
// Created by t-yabeny on 5/2/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SEMIABSTRACTALGORITHM_H
#define SHIP_STOWAGE_MODEL_SEMIABSTRACTALGORITHM_H


#include "AbstractAlgorithm.h"
#include "../../common/actors/ContainerShip.h"
#include "../../common/utils/Errors.h"

class SemiAbstractAlgorithm : public AbstractAlgorithm {  // implements the basics that each algorithm use
protected:
    ContainerShip ship;
    int algoErrors = ShipPlan_FatalError | ShipRoute_FatalError;  // we remove the flags after they are set

public:

    int readShipPlan(const std::string &shipPlanPath) override;

    int readShipRoute(const std::string &shipRoutePath) override;

    int setWeightBalanceCalculator(WeightBalanceCalculator &calculator) override;
};


#endif //SHIP_STOWAGE_MODEL_SEMIABSTRACTALGORITHM_H
