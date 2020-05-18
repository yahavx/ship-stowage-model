//
// Created by t-yabeny on 4/18/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATOR_H
#define SHIP_STOWAGE_MODEL_SIMULATOR_H

#include "../interfaces/AbstractAlgorithm.h"
#include "../common/actors/ContainerShip.h"
#include "../common/utils/Errors.h"
#include "SimulatorFileManager.h"
#include "../common/data_objects/Operations.h"
#include <memory>
#include <functional>


class Simulator {
    std::string travelRootDir;
    std::string algorithmsDir;
    std::string outputDir;
    SimulatorFileManager fileManager;
    std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> algorithmFactories;
    StringVector algorithmNames;  // the entry i corresponds to the name of the algorithm of factory i

public:

    // region Constructors

    Simulator(const std::string &travelRootDir, const std::string &algorithmsDir, const std::string &outputDir);

    Simulator(const std::string &travelRootDir, const std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> &algorithmFactories, const std::string &outputDir);  // for tests

    // endregion

    // region Simulation run

    /// Run a cartesian loop of “travel” X “algorithm”.
    void runSimulations();

private:
    /// Loads algorithms dynamically, returns false if no algorithm was loaded.
    void loadAlgorithmsDynamically(Errors &errors);

    /**
     * Simulates an algorithm on a single travel.
     * @return number of steps took to simulate, or -1 if the algorithm made at least 1 error.
     */
    int runSimulation(std::unique_ptr<AbstractAlgorithm> algorithmPtr);

    // endregion

    // region Constants

public:

    static const std::string s_resultsTableTitle;
    static const std::string s_sumColumnTitle;
    static const std::string s_errorsColumnTitle;

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_SIMULATOR_H
