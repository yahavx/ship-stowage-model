//
// Created by t-yabeny on 5/9/2020.
//

#include "AlgorithmRegistrar.h"

// region Singleton

AlgorithmRegistrar AlgorithmRegistrar::instance;

AlgorithmRegistrar &AlgorithmRegistrar::getInstance() {
    return instance;
}

// endregion

// region Registration

void AlgorithmRegistrar::registerAlgorithm(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithm) {
    algorithmsFactory.push_back(algorithm);
}

bool AlgorithmRegistrar::hasChanged() {
    if (algorithmsFactory.size() > size) {
        size = algorithmsFactory.size();
        return true;
    }

    return false;
}

std::function<std::unique_ptr<AbstractAlgorithm>()> AlgorithmRegistrar::getLast() {
    return algorithmsFactory.back();
}

// endregion
