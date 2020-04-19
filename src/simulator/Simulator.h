//
// Created by t-yabeny on 4/18/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATOR_H
#define SHIP_STOWAGE_MODEL_SIMULATOR_H

#include "../algorithms/stowage/IStowageAlgorithm.h"


class Simulator {

public:
    std::string staticOutputFile;

    Simulator();

    /**
     * Simulates an algorithm on a single travel.
     * @param travel a directory that contains Plan, Route, and list of <portId>_<number>.cargo_data files.
     */
    void runSimulation(IStowageAlgorithm &algorithm, const std::string &travel);

    /// Run a cartesian loop of “travel” X “algorithm”.
//    void runSimulations(std::vector<IStowageAlgorithm> algorithms, StringVector travels);

private:

    /**
     * Inits the ship of the simulation. Returns true if succeed.
     * @return true if succeed.
     */
    bool initSimulation(const std::string &shipPlanPath, const std::string &shipRoutePath, ContainerShip &ship) const;

    void initAlgorithm(IStowageAlgorithm &algorithm, const std::string &shipPlanPath,
                       const std::string &shipRoutePath) const;
};


#endif //SHIP_STOWAGE_MODEL_SIMULATOR_H
