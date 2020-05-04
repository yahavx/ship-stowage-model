//
// Created by t-yabeny on 5/4/2020.
//

#ifndef SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H
#define SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H

#pragma once

class WeightBalanceCalculator {

public:

    enum BalanceStatus {
        APPROVED , X_IMBALANCED , Y_IMBALANCED , X_Y_IMBALANCED
    };

// the method below is optional for ex2
//    int readShipPlan(const std::string& full_path_and_file_name);

    virtual BalanceStatus tryOperation(char loadUnload, int kg, int X, int Y) = 0;
};

#endif //SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H
