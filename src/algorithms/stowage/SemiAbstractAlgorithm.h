//
// Created by t-yabeny on 5/2/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SEMIABSTRACTALGORITHM_H
#define SHIP_STOWAGE_MODEL_SEMIABSTRACTALGORITHM_H


#include "AbstractAlgorithm.h"
#include "../../common/actors/ContainerShip.h"

class SemiAbstractAlgorithm : public AbstractAlgorithm {  // implements the basics that each algorithm use
protected:
    ContainerShip ship;
    int fatalError = 0;  // indicates that we couldn't initialize (fatal error in ship plan or route)

public:

    int readShipPlan(const std::string &shipPlanPath) override;

    int readShipRoute(const std::string &shipRoutePath) override;

    int setWeightBalanceCalculator(WeightBalanceCalculator &calculator) override;
};


#endif //SHIP_STOWAGE_MODEL_SEMIABSTRACTALGORITHM_H
