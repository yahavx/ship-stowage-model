#include <iostream>
#include "src/common/io/FileReader.h"
#include "src/common/io/ObjectsReader.h"
#include "src/utils/UtilFunctions.h"
#include "tests/SimulationTests.h"


int main(int argc, char **argv) {
//    simulationTest("../hw1/Travel_3");
//    simulationsTest({"../hw1/input/Travel_2", "../hw1/input/Travel_3"});
//    return 0;


    if (argc < 3) {
        std::cerr << "Insufficient arguments supplied. Please read the README for usage instructions. Program is terminated." << std::endl;
        return 1;
    }

    std::string outputDir = argv[1];

    if (!isDirectoryExists(outputDir)) {  // the output directory doesn't exist
        bool created = createFolder(outputDir);  // try to create it
        if (!created) {
            std::cerr << "Couldn't initialize output directory. Please read the README for usage instructions. Program is terminated." << std::endl;
            return 1;
        }
    }

    // Direct log prints to log files
    std::string outputLogFile = outputDir + "/output.txt";
    std::string errorLogFile =  outputDir + "/error.txt";
    freopen(outputLogFile.c_str(), "w", stdout);  // TODO
    freopen(errorLogFile.c_str(), "w", stderr);

    StringVector travels;

    for (int i = 2; i < argc; i++) {
        travels.push_back(argv[i]);
    }

    Simulator simulator;

    simulator.runSimulations(travels, outputDir);

    return 0;
}