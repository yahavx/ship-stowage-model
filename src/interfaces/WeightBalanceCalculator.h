//
// Created by t-yabeny on 5/4/2020.
//

#ifndef SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H
#define SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H

#pragma once

#include <string>

class WeightBalanceCalculator {

public:

    enum BalanceStatus {
        APPROVED , X_IMBALANCED , Y_IMBALANCED , X_Y_IMBALANCED
    };

    int readShipPlan(const std::string& full_path_and_file_name);

    BalanceStatus tryOperation(char loadUnload, int kg, int x, int y);
};

#endif //SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H
