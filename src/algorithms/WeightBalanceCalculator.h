//
// Created by yahav on 06/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H
#define SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H

class ContainerShip;  // pre declare

enum BalanceStatus{
    APPROVED,
    X_IMBALANCED,
    Y_IMBALANCED,
    X_Y_IMBALANCED
};

class WeightBalanceCalculator {

    /* TODO: Remove?
    bool isBalancedAfterLoad(ContainerShip &containerShip, int x, int y, int z);

    bool isBalancedAfterUnload(int x, int y, int z);
     */
    int tmp;

public:
    WeightBalanceCalculator(int tmp);
    BalanceStatus tryOperation(char loadUnload, int kg, int X, int Y);
};



#endif //SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H
