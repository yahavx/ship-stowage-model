//
// Created by t-yabeny on 4/18/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATOR_H
#define SHIP_STOWAGE_MODEL_SIMULATOR_H

#include "../algorithms/stowage/IStowageAlgorithm.h"


class Simulator {

public:
    std::string outputDir;
    std::vector<IStowageAlgorithm*> algorithms;

    Simulator(const std::string &outputDir);

    // region Simulation run

    /**
     * Simulates an algorithm on a single travel.
     * @param travel a directory that contains Plan, Route, and list of <portId>_<number>.cargo_data files.
     */
    StringStringVector runSimulation(IStowageAlgorithm &algorithm, const std::string &travel);

    /// Run a cartesian loop of “travel” X “algorithm”.
    void runSimulations(StringVector travels);
    // endregion

    // region Simulation init

private:

    /**
     * Inits the ship of a single simulation. Returns true if succeed.
     * @return true if succeed.
     */
    bool initSimulation(const std::string &shipPlanPath, const std::string &shipRoutePath, ContainerShip &ship, StringVector &errors) const;

    /// Inits the algorithm in a single simulation.
    void initAlgorithm(IStowageAlgorithm &algorithm, const std::string &shipPlanPath,
                       const std::string &shipRoutePath) const;

    /**
     * Perform packing operations received from algorithm, on simulator's ship. Validates each operation is legal.
     * @param errors empty vector, to fill with errors, one per each entry
     */
    void performPackingOperations(ContainerShip &ship, Port &port, const OPS &ops, StringVector &errors) const;
// endregion

    // region Constants

public:
    static const std::string s_instructionsFilename;

    static const std::string s_resultsTableTitle;
    static const std::string s_errorsTableTitle;
    static const std::string s_sumColumnTitle;
    static const std::string s_errorToken;
    // endregion
};


#endif //SHIP_STOWAGE_MODEL_SIMULATOR_H
