//
// Created by t-yabeny on 5/9/2020.
//

#include <iostream>
#include "AlgorithmRegistrar.h"
#include "../common/utils/UtilFunctions.h"

#ifdef RUNNING_ON_NOVA
#include <dlfcn.h>
#endif

// region Singleton

AlgorithmRegistrar AlgorithmRegistrar::instance;

AlgorithmRegistrar &AlgorithmRegistrar::getInstance() {
    return instance;
}

// endregion

// region Registration

void AlgorithmRegistrar::registerAlgorithm(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithm) {
    algorithmsFactory.push_back(algorithm);
    std::cout << "Added algorithm!" << std::endl;
}

int AlgorithmRegistrar::factoriesIncrease() {
    int delta = algorithmsFactory.size() - size;
    size = algorithmsFactory.size();
    return delta;
}

std::function<std::unique_ptr<AbstractAlgorithm>()> &AlgorithmRegistrar::getLast() {
    return algorithmsFactory.back();
}

ErrorFlag AlgorithmRegistrar::loadSharedObject(const std::string &path) {
#ifdef RUNNING_ON_NOVA
    std::unique_ptr<void, DlCloser> handle(dlopen(path, RTLD_LAZY));
    if (!handle) {
        return ErrorFlag::SharedObject_CantLoadSoFile;
        std::cout << "Load failed" << std::endl;
    }
    else {
        std::cout << "Load success" << std::endl;
    }
#endif
    (void)path;  // ignore unused parameter
    return ErrorFlag::Success;
}


// endregion
