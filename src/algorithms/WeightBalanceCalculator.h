//
// Created by yahav on 06/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H
#define SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H

#include "../data_structures/Cargo.h"

//class ContainerShip;  // pre declare

enum class BalanceStatus{
    APPROVED,
    X_IMBALANCED,
    Y_IMBALANCED,
    X_Y_IMBALANCED
};

class WeightBalanceCalculator {

    ShipPlan plan;

public:
    // region Constructors

    WeightBalanceCalculator();

    WeightBalanceCalculator(const ShipPlan &plan);
    // endregion

    // region Functions

    BalanceStatus tryOperation(Cargo &cargo, char loadUnload, int kg, int X, int Y);
    // endregion
};



#endif //SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H
