//
// Created by t-yabeny on 5/2/2020.
//

#ifndef SHIP_STOWAGE_MODEL_BADALGORITHM_H
#define SHIP_STOWAGE_MODEL_BADALGORITHM_H

#include "SemiAbstractAlgorithm.h"


class BadAlgorithm : public SemiAbstractAlgorithm {  // use this to test the simulator

public:

    int getInstructionsForCargo(const std::string &inputFile, const std::string &outputFile) override;

    std::string getAlgorithmName() override;
};


#endif //SHIP_STOWAGE_MODEL_BADALGORITHM_H
