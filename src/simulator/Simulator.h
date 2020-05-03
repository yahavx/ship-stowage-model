//
// Created by t-yabeny on 4/18/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATOR_H
#define SHIP_STOWAGE_MODEL_SIMULATOR_H

#include "../algorithms/stowage/AbstractAlgorithm.h"
#include "../common/actors/ContainerShip.h"
#include "../common/utils/Errors.h"
#include "SimulatorDataManager.h"
#include <memory>


class Simulator {

public:
    std::string outputDir;
    std::string travelRootDir;
    SimulatorDataManager dataManager;
    std::vector<std::shared_ptr<AbstractAlgorithm>> algorithms;

    // region Constructors

    Simulator(const std::string &outputDir, const std::string &travelRootDir);
    // endregion

    // region Simulation run

private:
    /**
     * Simulates an algorithm on a single travel.
     * @return number of steps took to simulate, or -1 if the algorithm made at least 1 error.
     */
    int runSimulation(AbstractAlgorithm &algorithm);

public:
    /// Run a cartesian loop of “travel” X “algorithm”.
    void runSimulations();

    // endregion

    // region Simulation init

private:

     /// Inits the ship of a single simulation. Assume no fatal errors are in the plan and route.
    ContainerShip initSimulation(Errors &errors);

    /// Inits the algorithm in a single simulation.
    void initAlgorithm(AbstractAlgorithm &algorithm, Errors &errors);

    /**
     * Perform packing operations received from algorithm, on simulator's ship. Validates each operation is legal.
     * @param errors empty vector, to fill with errors, one per each entry
     */
    void performPackingOperations(ContainerShip &ship, Port &port, const Operations &ops, Errors &errors) const;

    void validatePackingOperation(ContainerShip &ship, Port &port, const PackingOperation &op, Errors &errors) const;
// endregion

    // region Constants

public:
    static const std::string s_instructionsFilename;

    static const std::string s_resultsTableTitle;
    static const std::string s_generalErrorsTableName;

    static const std::string s_generalErrorsRowTitle;
    static const std::string s_sumColumnTitle;
    static const std::string s_errorsColumnTitle;

    static const std::string s_errorToken;

    static const std::string s_noTravelPathSuppliedError;
    // endregion
};


#endif //SHIP_STOWAGE_MODEL_SIMULATOR_H
