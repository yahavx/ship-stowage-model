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
    bool fatalError = false;  // indicates that we couldn't initialize (fatal error in ship plan or route)

public:

    int readShipPlan(const std::string &shipPlanPath) override;

    int readShipRoute(const std::string &shipRoutePath) override;

    int setWeightBalanceCalculator(WeightBalanceCalculator &calculator) override;

    int getInstructionsForCargo(const std::string &inputFile, const std::string &outputFile) override;

    std::string getAlgorithmName() override;

private:

    bool initPortId(const std::string &inputFile, Port &port) const;

    void initializePort(const std::string &inputFile, Port &port) const;
};


#endif //SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H
