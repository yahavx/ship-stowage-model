//
// Created by t-yabeny on 5/5/2020.
//

#include <iostream>
#include "AlgorithmRegistration.h"
#include "../simulator/AlgorithmRegistrar.h"


AlgorithmRegistration::AlgorithmRegistration(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
    AlgorithmRegistrar::getInstance().registerAlgorithm(algorithmFactory);
}
