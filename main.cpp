#include <iostream>
#include "src/common/io/FileReader.h"
#include "src/common/io/ObjectsReader.h"
#include "tests/SimulationTests.h"


int main(int argc, char **argv) {
//        simulationTest("../input-examples\\Travel_4");  // TODO: remove at the end
//    return 0;

    if (argc == 1) {
        std::cout << "No arguments supplied. Please read the README for usage instructions. Program is terminated." << std::endl;
        return 1;
    }

    // Direct log prints to log files
    freopen( "../simulation-output/output.txt", "w", stdout );  // TODO
    freopen( "../simulation-output/error.txt", "w", stderr );

    StringVector travels;

    for (int i = 1; i < argc; i++){
        travels.push_back(argv[i]);
    }

    Simulator simulator;

    simulator.runSimulations(travels);

    return 0;
}
