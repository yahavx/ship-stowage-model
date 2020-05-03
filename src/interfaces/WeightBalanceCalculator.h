//
// Created by t-yabeny on 5/4/2020.
//

#ifndef SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H
#define SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H

#pragma once

enum BalanceStatus {  // TODO: the enum should be inside the class
    APPROVED , X_IMBALANCED , Y_IMBALANCED , X_Y_IMBALANCED
};

class WeightBalanceCalculator {

public:

// the method below is optional for ex2
    int readShipPlan(const std::string& full_path_and_file_name);

    BalanceStatus tryOperation(char loadUnload, int kg, int X, int Y);
};

#endif //SHIP_STOWAGE_MODEL_WEIGHTBALANCECALCULATOR_H
