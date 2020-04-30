//
// Created by yahav on 06/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H
#define SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H

#include "../common/data_structures/Cargo.h"

//class ContainerShip;  // pre declare

enum class BalanceStatus{
    APPROVED,
    X_IMBALANCED,
    Y_IMBALANCED,
    X_Y_IMBALANCED
};

class WeightBalanceCalculator {

    ShipPlan plan;
    Cargo *cargo;

public:
    // region Constructors

    WeightBalanceCalculator();

    WeightBalanceCalculator(const ShipPlan &plan);
    // endregion

    // region Getters and setters

    Cargo &getCargo() const;

    const ShipPlan &getPlan() const;

    void setPlan(const ShipPlan &plan);

    void setCargo(Cargo &cargo);
    // endregion

    // region Functions

    BalanceStatus tryOperation(char loadUnload, int kg, int X, int Y);
    // endregion
};



#endif //SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H
