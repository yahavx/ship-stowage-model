//
// Created by t-yabeny on 4/19/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATIONTESTS_H
#define SHIP_STOWAGE_MODEL_SIMULATIONTESTS_H


#include "../src/simulator/Simulator.h"
#include "../src/algorithms/stowage/NaiveStowageAlgorithm.h"
#include "IOTests.h"
#include "../src/simulator/SimulatorUtil.h"


void inline tableResultSortTest(){

    StringStringVector d = {
            {"title", "row1", "row2", "row3", "sum", "errors"},
            {"algo1", "2", "-1", "15", "17", "1"},
            {"algo2", "10", "2", "18", "30", "0"},
            {"algo3", "2", "12", "-1", "14", "1"}
    };

    cout << d;

    sortResultsTable(d);

    cout << "Sorting..." << endl;

    cout << d;


}


#endif //SHIP_STOWAGE_MODEL_SIMULATIONTESTS_H
