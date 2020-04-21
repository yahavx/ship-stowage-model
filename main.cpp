#include <iostream>
#include "src/common/io/FileReader.h"
#include "src/common/io/ObjectsReader.h"
#include "src/utils/UtilFunctions.h"
#include "tests/SimulationTests.h"


int main(int argc, char **argv) {
//    simulationTest("../hw1/Travel_3");
    simulationsTest({"../hw1/input/Travel_2", "../hw1/input/Travel_3"});
    return 0;

    if (argc < 3) {
        std::cerr << "Insufficient arguments supplied. Please read the README for usage instructions. Program is terminated." << std::endl;
        return 1;
    }

    bool created = createFolder(argv[1]);  // first argument is the path  // TODO: inject the path to the
    if (!created) {
        std::cerr << "Couldn't initialize output directory. Please read the README for usage instructions. Program is terminated." << std::endl;
        return 1;
    }

    // Direct log prints to log files
    freopen("../simulation-output/output.txt", "w", stdout);  // TODO
    freopen("../simulation-output/error.txt", "w", stderr);

    StringVector travels;

    for (int i = 2; i < argc; i++) {
        travels.push_back(argv[i]);
    }

    Simulator simulator;

    simulator.runSimulations(travels);

    return 0;
}