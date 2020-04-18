//
// Created by Orr on 4/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H
#define SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H


#include "IStowageAlgorithm.h"

/**
 * A naive implementation.
 */
class NaiveStowageAlgorithm : public IStowageAlgorithm {

public:

    void setWeightBalanceCalculator(WeightBalanceCalculator &calculator) override;

    void getInstructionsForCargo(const std::string &inputFile,
                                 const std::string &outputFile) override;

    void setShipPlanFromPath(const std::string &shipPlanPath) override;

    void setShipRouteFromPath(const std::string &shipRoutePath) override;
};


#endif //SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H
