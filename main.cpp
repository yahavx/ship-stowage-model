#include <iostream>
#include "src/common/io/FileReader.h"
#include "src/common/io/ObjectsReader.h"
#include "src/common/utils/UtilFunctions.h"
#include "src/simulator/Simulator.h"
#include "src/simulator/SimulatorUtil.h"
#include "tests/IOTests.h"
#include "tests/SimulationTests.h"

void parseCmdArguments(int argc, char **argv, std::string &travelPath, std::string &algorithmPath, std::string &outputPath);

int main(int argc, char **argv) {
//    runIOTests();
//    tableResultSortTest();
//    return 0;

    if (argc != 3 && argc != 5 && argc != 7) {
        std::cerr << "Insufficient arguments supplied. Please read the README for usage instructions. Program is terminated." << std::endl;
        return 1;
    }

    std::string travelPath = "", algorithmPath = "", outputPath = "";

    parseCmdArguments(argc, argv, travelPath, algorithmPath, outputPath);

    bool created = createFolder(outputPath);
    if (!created) {
        std::cerr << "Couldn't initialize output directory. Program is terminated." << std::endl;
        return 1;
    }

    travelPath = "../input-examples/single-travel-difference";

    Simulator simulator(travelPath, algorithmPath, outputPath);

    simulator.runSimulations();

    return 0;
}

void parseCmdArguments(int argc, char **argv, std::string &travelPath, std::string &algorithmPath, std::string &outputPath) {
    for (int i = 1; i < argc; i += 2) {
        std::string flag = argv[i];

        if (flag == "-travel_path") {
            travelPath = argv[i + 1];
        }

        if (flag == "-algorithm_path") {
            algorithmPath = argv[i + 1];
        }

        if (flag == "-output") {
            outputPath = argv[i + 1];
        }

    }
}