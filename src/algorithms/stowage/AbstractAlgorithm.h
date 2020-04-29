//
// Created by Orr on 4/9/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ABSTRACTALGORITHM_H
#define SHIP_STOWAGE_MODEL_ABSTRACTALGORITHM_H

#pragma once

#include "../WeightBalanceCalculator.h"

class AbstractAlgorithm {

public:

    virtual void setWeightBalanceCalculator(WeightBalanceCalculator& calculator) = 0;

    virtual void getInstructionsForCargo(const std::string& inputFileName,
                                         const std::string& outputFileName) = 0;

    virtual void readShipPlan(const std::string& shipPlanPath) = 0 ;

    virtual void readShipRoute(const std::string& shipRoutePath) = 0;  // TODO: change shipRoutePath (and shipPlanPath) to full_path_and_file_name?

    virtual std::string getAlgorithmName() = 0;  // TODO: remove
};


#endif //SHIP_STOWAGE_MODEL_ABSTRACTALGORITHM_H
