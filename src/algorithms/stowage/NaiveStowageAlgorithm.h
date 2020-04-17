//
// Created by Orr on 4/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H
#define SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H


#include "StowageAlgorithmInterface.h"

class NaiveStowageAlgorithm : StowageAlgorithmInterface {

public:

    void readShipPlan(const std::string& filePath) override;

    void readShipRoute(const std::string& filePath) override;

    void setWeightBalanceCalculator(WeightBalanceCalculator& calculator) override;

    void getInstructionsForCargo(const std::string& inputFileName,
                                 const std::string& outputFileName) override;
};


#endif //SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H
