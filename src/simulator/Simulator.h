//
// Created by t-yabeny on 4/18/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATOR_H
#define SHIP_STOWAGE_MODEL_SIMULATOR_H

#include "../algorithms/stowage/AbstractAlgorithm.h"
#include "../common/actors/ContainerShip.h"
#include "../common/utils/ErrorFlags.h"


class Simulator {

public:
    std::string outputDir;
    std::vector<AbstractAlgorithm*> algorithms;

    // region Constructors

    explicit Simulator(const std::string &outputDir);
    // endregion

    // region Simulation run

    /**
     * Simulates an algorithm on a single travel.
     * @param travel a directory that contains Plan, Route, and list of <portId>_<number>.cargo_data files.
     */
    StringStringVector runSimulation(AbstractAlgorithm &algorithm, const std::string &travel, const std::string &craneOutputDir);

    /** Run a cartesian loop of “travel” X “algorithm”.
     *
     * @param travels directory that contains sub-directories, each one is a travel
     */
    void runSimulations(const std::string &travelPath);

    // endregion

    // region Simulation init

private:

     /// Inits the ship of a single simulation. Assume no fatal errors are in the plan and route.
    ContainerShip initSimulation(const std::string &shipPlanPath, const std::string &shipRoutePath, std::vector<ErrorFlag> &errors) const;

    /// Inits the algorithm in a single simulation.
    void initAlgorithm(AbstractAlgorithm &algorithm, const std::string &shipPlanPath, const std::string &shipRoutePath) const;

    /**
     * Perform packing operations received from algorithm, on simulator's ship. Validates each operation is legal.
     * @param errors empty vector, to fill with errors, one per each entry
     */
    void performPackingOperations(ContainerShip &ship, Port &port, const Operations &ops, StringVector &errors) const;

    void validatePackingOperation(ContainerShip &ship, Port &port, const PackingOperation &op, StringVector &errors) const;
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
