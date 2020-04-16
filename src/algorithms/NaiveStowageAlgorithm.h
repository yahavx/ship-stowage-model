//
// Created by Orr on 4/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H
#define SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H


#include "StowageAlgorithmInterface.h"

class NaiveStowageAlgorithm : StowageAlgorithmInterface {

public:

    void readShipPlan(const std::string& full_path_and_file_name) override;

    void readShipRoute(const std::string& full_path_and_file_name) override;

    void setWeightBalanceCalculator(WeightBalanceCalculator& calculator) override;

    void getInstructionsForCargo(const std::string& input_full_path_and_file_name,
                                 const std::string& output_full_path_and_file_name) override;
};


#endif //SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H
