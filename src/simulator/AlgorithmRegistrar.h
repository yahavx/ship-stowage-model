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

class AlgorithmRegistrar {
    static AlgorithmRegistrar instance;
    std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> algorithmsFactory;
    longUInt size = 0;

    AlgorithmRegistrar() = default;

public:

    void registerAlgorithm(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithm);

    static AlgorithmRegistrar &getInstance();

    /// Returns true if the number of algorithms in the vector has changed (increased) since the last call.
    bool hasChanged();

    /// Returns the last algorithm to be pushed.
    std::function<std::unique_ptr<AbstractAlgorithm>()> getLast();
};


#endif //SHIP_STOWAGE_MODEL_ALGORITHMREGISTRAR_H
