//
// Created by t-yabeny on 4/18/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATOR_H
#define SHIP_STOWAGE_MODEL_SIMULATOR_H

#include "../interfaces/AbstractAlgorithm.h"
#include "../common/actors/ContainerShip.h"
#include "../common/utils/Errors.h"
#include "SimulatorDataManager.h"
#include <memory>
#include <functional>


class Simulator {
    std::string travelRootDir;
    std::string algorithmsDir;
    std::string outputDir;
    SimulatorDataManager dataManager;
    std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> algorithmFactories;
    StringVector algorithmNames;  // the entry i corresponds to the name of the algorithm of factory i

public:

    // region Constructors

    Simulator(const std::string &travelRootDir, const std::string &algorithmsDir, const std::string &outputDir);

    // endregion

    // region Simulation run

    /// Run a cartesian loop of “travel” X “algorithm”.
    void runSimulations();

private:
    /**
     * Simulates an algorithm on a single travel.
     * @return number of steps took to simulate, or -1 if the algorithm made at least 1 error.
     */
    int runSimulation(std::unique_ptr<AbstractAlgorithm> algorithmPtr);

    /// Loads algorithms dynamically, returns false if no algorithm was loaded.
    void loadAlgorithmsDynamically(Errors &errors);

    // endregion

private:

    // region Simulation init

    /// Inits the ship of a single simulation. Assume no fatal errors are in the plan and route.
    ContainerShip initSimulation(WeightBalanceCalculator &calculator, Errors &errors);

    /// Inits the algorithm in a single simulation. Returns the error return value of the algorithm.
    int initAlgorithm(AbstractAlgorithm *algorithm, WeightBalanceCalculator &calculator, Errors &errors);

    /**
     * Perform packing operations received from algorithm, on simulator's ship. Validates each operation is legal.
     * @param errors empty vector, to fill with errors, one per each entry
     */

    // endregion

    // region Perform operations

    void performPackingOperations(ContainerShip &ship, Port &port, const Operations &ops, Errors &errors) const;

    // endregion

public:

    // region Constants

public:

    static const std::string s_resultsTableTitle;
    static const std::string s_sumColumnTitle;
    static const std::string s_errorsColumnTitle;

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_SIMULATOR_H
