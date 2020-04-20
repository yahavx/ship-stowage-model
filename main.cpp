#include <iostream>
#include "src/common/io/FileReader.h"
#include "src/utils/UtilFunctions.h"
#include "src/common/io/ObjectsReader.h"
#include <cassert>
#include <fstream>
#include "src/actors/Port.h"
#include "src/utils/Printers.h"
#include "tests/IOTests.h"
#include "tests/SimulationTests.h"
#include "src/simulator/Simulator.h"
#include "src/algorithms/stowage/NaiveStowageAlgorithm.h"
#include "src/simulator/SimulatorUtil.h"

using namespace std;  // TODO: remove this and all redundant includes


int main(int argc, char **argv) {
    //    simulationTest("../input-examples\\Travel_4");  // TODO: remove at the end
//    return 0;

    if (argc == 1) {
        cout << "No arguments supplied. Please read the README for usage instructions. Program is terminated." << endl;
        return 1;
    }

    //Direct log prints to log files
    freopen( "../simulation-output/output.txt", "w", stdout );
    freopen( "../simulation-output/error.txt", "w", stderr );

    StringVector travels;

    for (int i = 1; i < argc; i++){
        travels.push_back(argv[i]);
    }

    Simulator simulator;

    simulator.runSimulations(travels);

    return 0;
}