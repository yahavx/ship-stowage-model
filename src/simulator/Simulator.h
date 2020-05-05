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


class Simulator {
    std::string outputDir;
    std::string travelRootDir;
    SimulatorDataManager dataManager;
    std::vector<std::shared_ptr<AbstractAlgorithm>> algorithms;

public:

    // region Constructors

    Simulator(const std::string &outputDir, const std::string &travelRootDir);

    // endregion

    // region Simulation run

    /// Run a cartesian loop of “travel” X “algorithm”.
    void runSimulations();

private:
    /**
     * Simulates an algorithm on a single travel.
     * @return number of steps took to simulate, or -1 if the algorithm made at least 1 error.
     */
    int runSimulation(AbstractAlgorithm &algorithm);

    // endregion

private:

    // region Simulation init

     /// Inits the ship of a single simulation. Assume no fatal errors are in the plan and route.
    ContainerShip initSimulation(WeightBalanceCalculator &calculator, Errors &errors);

    /// Inits the algorithm in a single simulation.
    void initAlgorithm(AbstractAlgorithm &algorithm, WeightBalanceCalculator &calculator, Errors &errors);

    /**
     * Perform packing operations received from algorithm, on simulator's ship. Validates each operation is legal.
     * @param errors empty vector, to fill with errors, one per each entry
     */

    // endregion

    // region Perform operations and validations

    void performPackingOperations(ContainerShip &ship, Port &port, const Operations &ops, Errors &errors) const;

    void validatePackingOperation(ContainerShip &ship, Port &port, const PackingOperation &op, Errors &errors) const;

    /// Returns true if travel can be used (i.e. ShipRoute and ShipPlan doesn't contain a fatal error).

    bool isTravelValid(Errors &errors = Errors::garbageCollector);

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
