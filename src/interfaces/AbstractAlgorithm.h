//
// Created by Orr on 4/9/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ABSTRACTALGORITHM_H
#define SHIP_STOWAGE_MODEL_ABSTRACTALGORITHM_H

#pragma once

#include <string>

// forward declaration
class WeightBalanceCalculator;

class AbstractAlgorithm {
public:
    virtual ~AbstractAlgorithm(){}

    enum class Action { LOAD = 'L', UNLOAD = 'U', MOVE = 'M', REJECT = 'R'};

    virtual int readShipPlan(const std::string& full_path_and_file_name) = 0 ;

    virtual int readShipRoute(const std::string& full_path_and_file_name) = 0;  // TODO: change shipRoutePath (and shipPlanPath) to full_path_and_file_name?

    virtual int setWeightBalanceCalculator(WeightBalanceCalculator& calculator) = 0;

    virtual int getInstructionsForCargo(const std::string& input_full_path_and_file_name, const std::string& output_full_path_and_file_name) = 0;

    virtual std::string getAlgorithmName() = 0;  // TODO: remove
};


#endif //SHIP_STOWAGE_MODEL_ABSTRACTALGORITHM_H
