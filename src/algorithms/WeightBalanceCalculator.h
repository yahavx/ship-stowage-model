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

    Cargo cargo;

public:
    BalanceStatus tryOperation(char loadUnload, int kg, int X, int Y);

    Cargo &getCargo();

    void setCargo(Cargo &cargo);
};



#endif //SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H
