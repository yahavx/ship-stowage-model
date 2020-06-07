//
// Created by Orr on 6/7/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ALGORITHMTRAVELTASK_H
#define SHIP_STOWAGE_MODEL_ALGORITHMTRAVELTASK_H

#include<string>
#include <bits/unique_ptr.h>
#include <functional>
#include "SimulatorFileManager.h"
#include "../interfaces/AbstractAlgorithm.h"

class AlgorithmTravelTask {

    // Resource managers
    SimulatorFileManager fileManager;
    Tracer &tracer;

    // Results table and indices
    StringStringVector &resultsTable;
    std::pair<longUInt, longUInt> resultsTableIndices;

    // Travel and algorithm
    std::function<std::unique_ptr<AbstractAlgorithm>()> &algorithmFactory;
    std::string &travel;

public:
    AlgorithmTravelTask(SimulatorFileManager fileManager, Tracer &tracer,
                        StringStringVector &resultsTable, std::pair<longUInt, longUInt> resultsTableIndices,
                        std::function<std::unique_ptr<AbstractAlgorithm>()> &algorithmFactory, std::string &travel);

    void run();
};


#endif //SHIP_STOWAGE_MODEL_ALGORITHMTRAVELTASK_H
