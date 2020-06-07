//
// Created by t-yabeny on 4/19/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATIONTESTS_H
#define SHIP_STOWAGE_MODEL_SIMULATIONTESTS_H


#include "../src/simulator/Simulator.h"
#include "../src/algorithms/NaiveStowageAlgorithm.h"
#include "IOTests.h"
#include "../src/simulator/SimulatorUtil.h"
#include "assert.h"
#undef NDEBUG

#define _unused(x) ((void)(x))

void inline tableResultSortTest();

void inline testSimulation(std::string travelPath, std::string algorithmPath, std::string outputPath, StringStringVector expected);

void inline simulationTests() {
    tableResultSortTest();
    printSeparator(1, 1);

    StringStringVector expected =
            {{"RESULTS", "LongTrip", "Sum", "Num Errors"},
             {"Naive", "10", "10", "0"}};
    testSimulation("../input-examples/single-travel", "", "../simulation-test-output", expected);
}


void inline tableResultSortTest(){
    StringStringVector actual = {
            {"title", "row1", "row2", "row3", "sum", "errors"},
            {"algo1", "2", "-1", "15", "17", "1"},
            {"algo2", "10", "2", "18", "30", "0"},
            {"algo3", "2", "12", "-1", "14", "1"}
    };

    cout << actual;

    sortResultsTable(actual);

    StringStringVector expected = {
            {"title", "row1", "row2", "row3", "sum", "errors"},
            {"algo2", "10", "2", "18", "30", "0"},
            {"algo3", "2", "12", "-1", "14", "1"},
            {"algo1", "2", "-1", "15", "17", "1"}
    };

    cout << "Sorting..." << endl;

    cout << actual;

    assert(actual == expected);
}

void inline testSimulation(std::string travelPath, std::string algorithmPath, std::string outputPath, StringStringVector expected) {
    Simulator simulator(travelPath, algorithmPath, outputPath, 1);
    simulator.runSimulations();

    StringStringVector actual = readFile(outputPath + "/simulation.results");
    assert(actual == expected);
    _unused(expected);
}


#endif //SHIP_STOWAGE_MODEL_SIMULATIONTESTS_H
