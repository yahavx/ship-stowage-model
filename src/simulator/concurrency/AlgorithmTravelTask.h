//
// Created by Orr on 6/7/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ALGORITHMTRAVELTASK_H
#define SHIP_STOWAGE_MODEL_ALGORITHMTRAVELTASK_H

#include<string>
#include <bits/unique_ptr.h>
#include <functional>
#include "../SimulatorFileManager.h"
#include "../../interfaces/AbstractAlgorithm.h"

/**
 * Task for thread to complete.
 * Defines the run of an algorithm on a specific travel.
 */
class AlgorithmTravelTask {

    // region Attributes

    // Resource managers
    SimulatorFileManager fileManager;
    Tracer &tracer;

    // Results table and indices
    StringStringVector &resultsTable;
    std::pair<longUInt, longUInt> resultsTableIndices;

    // Travel and algorithm
    std::function<std::unique_ptr<AbstractAlgorithm>()> &algorithmFactory;
    std::string &travel;

    // endregion

public:

    // region Constructors

    AlgorithmTravelTask(SimulatorFileManager fileManager, Tracer &tracer,
                        StringStringVector &resultsTable, std::pair<longUInt, longUInt> resultsTableIndices,
                        std::function<std::unique_ptr<AbstractAlgorithm>()> &algorithmFactory, std::string &travel);

    // endregion

    // region Api

    /**
     * Execute the task: start the algorithm with the given travel as input
     * Writes te result to the given indices in the results table
     */
    void run();

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_ALGORITHMTRAVELTASK_H
