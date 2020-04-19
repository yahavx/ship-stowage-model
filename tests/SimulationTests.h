//
// Created by t-yabeny on 4/19/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATIONTESTS_H
#define SHIP_STOWAGE_MODEL_SIMULATIONTESTS_H


#include "../src/simulator/Simulator.h"
#include "../src/algorithms/stowage/NaiveStowageAlgorithm.h"

using namespace std;


void inline simulationTest(){
    Simulator simulator;

    NaiveStowageAlgorithm naiveStowageAlgorithm;
    IStowageAlgorithm &algorithm(naiveStowageAlgorithm);

    string path = "../input-examples/Travel_3";

    simulator.runSimulation(algorithm, path);
}

#endif //SHIP_STOWAGE_MODEL_SIMULATIONTESTS_H