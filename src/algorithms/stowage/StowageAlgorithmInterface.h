//
// Created by Orr on 4/9/2020.
//

#ifndef SHIP_STOWAGE_MODEL_STOWAGEALGORITHMINTERFACE_H
#define SHIP_STOWAGE_MODEL_STOWAGEALGORITHMINTERFACE_H


#include <map>
#include "../../actors/ContainerShip.h"

/**
 * Algorithm that for given
 */
class StowageAlgorithmInterface {

protected:
    ContainerShip ship;
public:

    virtual void readShipPlan(const std::string& full_path_and_file_name) = 0;

    virtual void readShipRoute(const std::string& full_path_and_file_name) = 0;

    virtual void setWeightBalanceCalculator(WeightBalanceCalculator& calculator) = 0;

    virtual void getInstructionsForCargo(const std::string& input_full_path_and_file_name,
                                         const std::string& output_full_path_and_file_name) = 0;
};


#endif //SHIP_STOWAGE_MODEL_STOWAGEALGORITHMINTERFACE_H
