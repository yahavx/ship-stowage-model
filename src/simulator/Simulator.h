//
// Created by t-yabeny on 4/18/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATOR_H
#define SHIP_STOWAGE_MODEL_SIMULATOR_H

#include "../algorithms/stowage/IStowageAlgorithm.h"


class Simulator {

public:
    std::string staticOutputFile;
    std::vector<IStowageAlgorithm*> algorithms;  // TODO: init dynamically? (Gittik style)

    Simulator();

    /**
     * Simulates an algorithm on a single travel.
     * @param travel a directory that contains Plan, Route, and list of <portId>_<number>.cargo_data files.
     */
    void runSimulation(IStowageAlgorithm &algorithm, const std::string &travel);

    /// Run a cartesian loop of “travel” X “algorithm”.
    void runSimulations(StringVector travels);

private:

    /**
     * Inits the ship of a single simulation. Returns true if succeed.
     * @return true if succeed.
     */
    bool initSimulation(const std::string &shipPlanPath, const std::string &shipRoutePath, ContainerShip &ship) const;

    /// Inits the algorithm in a single simulation.
    void initAlgorithm(IStowageAlgorithm &algorithm, const std::string &shipPlanPath,
                       const std::string &shipRoutePath) const;

    /// Perform packing operations received from algorithm, on simulator's ship. Validates each operation is legal.
    void performPackingOperations(ContainerShip &ship, Port &port, const OPS &ops) const;
};


#endif //SHIP_STOWAGE_MODEL_SIMULATOR_H
