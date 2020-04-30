//
// Created by Orr on 4/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H
#define SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H


#include "AbstractAlgorithm.h"
#include "../../common/actors/ContainerShip.h"


/**
 * A naive implementation.
 */
class NaiveStowageAlgorithm : public AbstractAlgorithm {
protected:
    ContainerShip ship;

public:

    void setWeightBalanceCalculator(WeightBalanceCalculator &calculator) override;

    void getInstructionsForCargo(const std::string &inputFile, const std::string &outputFile) override;

    void readShipPlan(const std::string &shipPlanPath) override;

    void readShipRoute(const std::string &shipRoutePath) override;

    std::string getAlgorithmName() override;

private:

    bool initPortId(const std::string &inputFile, Port &port) const;

    void initializePort(const std::string &inputFile, Port &port) const;
};


#endif //SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H
