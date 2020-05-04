//
// Created by yahav on 06/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_NAIVEWEIGHTBALANCER_H
#define SHIP_STOWAGE_MODEL_NAIVEWEIGHTBALANCER_H

#include "../data_structures/Cargo.h"
#include "../../interfaces/WeightBalanceCalculator.h"
#include "AbstractWeightBalancer.h"

class NaiveWeightBalancer : public AbstractWeightBalancer {


public:
    // region Constructors

    NaiveWeightBalancer();

    NaiveWeightBalancer(const ShipPlan &plan);

    // endregion

    // region Functions

    WeightBalanceCalculator::BalanceStatus tryOperation(char loadUnload, int kg, int X, int Y);

    // endregion
};



#endif //SHIP_STOWAGE_MODEL_NAIVEWEIGHTBALANCER_H
