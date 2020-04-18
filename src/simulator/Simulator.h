//
// Created by t-yabeny on 4/18/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATOR_H
#define SHIP_STOWAGE_MODEL_SIMULATOR_H

#include "../algorithms/stowage/IStowageAlgorithm.h"


class Simulator {

public:
    /**
     * Simulates an algorithm on a single travel.
     * @param travel a directory that contains Plan, Route, and list of <portId>_<number>.cargo_data files.
     */
    void runSimulation(IStowageAlgorithm &algorithm, const std::string &travel);

    /// Run a cartesian loop of “travel” X “algorithm”.
//    void runSimulations(std::vector<IStowageAlgorithm> algorithms, StringVector travels);
};


#endif //SHIP_STOWAGE_MODEL_SIMULATOR_H
