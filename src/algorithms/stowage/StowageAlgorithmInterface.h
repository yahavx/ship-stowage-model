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

    virtual void readShipPlan(const std::string& filePath) = 0;

    virtual void readShipRoute(const std::string& filePath) = 0;

    virtual void setWeightBalanceCalculator(WeightBalanceCalculator& calculator) = 0;

    virtual void getInstructionsForCargo(const std::string& inputFileName,
                                         const std::string& outputFileName) = 0;
};


#endif //SHIP_STOWAGE_MODEL_STOWAGEALGORITHMINTERFACE_H
