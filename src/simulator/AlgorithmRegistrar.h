//
// Created by t-yabeny on 5/9/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ALGORITHMREGISTRAR_H
#define SHIP_STOWAGE_MODEL_ALGORITHMREGISTRAR_H

#include <vector>
#include <memory>
#include <functional>
#include "../interfaces/AbstractAlgorithm.h"
#include "../common/utils/Definitions.h"
#include "../common/utils/Errors.h"
#ifdef RUNNING_ON_NOVA

#include "iostream"
#include <dlfcn.h>
#endif

class AlgorithmRegistrar {
    static AlgorithmRegistrar instance;
    std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> algorithmsFactory;
    longUInt size = 0;

    AlgorithmRegistrar() = default;  // singleton class, private constructor

public:

    void registerAlgorithm(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithm);

    static AlgorithmRegistrar &getInstance();

    /// Returns the delta of factory size since the last call (0 or greater, should be 1 in a successful call).
    int factoriesIncrease();

    /// Returns the last algorithm to be pushed.
    std::function<std::unique_ptr<AbstractAlgorithm>()> &getLast();

    /// Loads SO file.
    ErrorFlag loadSharedObject(const std::string &path);

#ifdef RUNNING_ON_NOVA
private:
    struct DlCloser{
        void operator()(void *dlHandle) const noexcept {
//            std::cout << "Closing handle" << std::endl;
            (void) dlHandle;
//            dlclose(dlHandle);  // TODO: uncomment after solving duplicates problem
        }
    };

    std::vector<std::unique_ptr<void, DlCloser>> handles;  // We keep them here to call the destructor at the end
#endif
};


#endif //SHIP_STOWAGE_MODEL_ALGORITHMREGISTRAR_H
