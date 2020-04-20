//
// Created by Orr on 4/9/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ISTOWAGEALGORITHM_H
#define SHIP_STOWAGE_MODEL_ISTOWAGEALGORITHM_H


#include <map>
#include "../../actors/ContainerShip.h"

/**
 * The basic algorithm that is supplied to the simulator.
 */
class IStowageAlgorithm {

protected:
    ContainerShip ship;

public:

    virtual void setWeightBalanceCalculator(WeightBalanceCalculator& calculator) = 0;

    virtual void getInstructionsForCargo(const std::string& inputFileName,
                                         const std::string& outputFileName) = 0;

    virtual void setShipPlanFromPath(const std::string& shipPlanPath) = 0 ;

    virtual void setShipRouteFromPath(const std::string& shipRoutePath) = 0;

    virtual std::string getAlgorithmName() = 0;
};


#endif //SHIP_STOWAGE_MODEL_ISTOWAGEALGORITHM_H
