//
// Created by Orr on 4/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H
#define SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H


#include "../interfaces/AbstractAlgorithm.h"
#include "../common/actors/ContainerShip.h"
#include "SemiAbstractAlgorithm.h"
#include "../interfaces/AlgorithmRegistration.h"

/**
 * A naive implementation.
 */
class NaiveStowageAlgorithm : public SemiAbstractAlgorithm {

public:
    int getInstructionsForCargo(const std::string &inputFile, const std::string &outputFile) override;

    std::string getAlgorithmName() override;
};


#endif //SHIP_STOWAGE_MODEL_NAIVESTOWAGEALGORITHM_H
