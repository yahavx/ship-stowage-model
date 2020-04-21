//
// Created by t-yabeny on 4/19/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATIONTESTS_H
#define SHIP_STOWAGE_MODEL_SIMULATIONTESTS_H


#include "../src/simulator/Simulator.h"
#include "../src/algorithms/stowage/NaiveStowageAlgorithm.h"


void inline simulationTest(std::string path = "../input-examples\\Travel_4"){
    Simulator simulator;

    NaiveStowageAlgorithm naiveStowageAlgorithm;
    IStowageAlgorithm &algorithm(naiveStowageAlgorithm);

    simulator.runSimulation(algorithm, path);
}

void inline simulationsTest(StringVector path = StringVector()){
    Simulator simulator;

    if (path.size() == 0) {
        path = {"../input-examples/Travel_1", "../input-examples/Travel_2", "../input-examples/Travel_3", "../input-examples/Travel_4",
                "../input-examples/Travel_5", "../input-examples/Travel_X"};
    }

    simulator.runSimulations(path, "../simulation-output");
}


#endif //SHIP_STOWAGE_MODEL_SIMULATIONTESTS_H
