#include <iostream>
#include "../common/io/FileReader.h"
#include "../common/io/ObjectsReader.h"
#include "../common/utils/UtilFunctions.h"
#include "Simulator.h"
#include "SimulatorUtil.h"
#ifndef RUNNING_ON_NOVA
#include "../../tests/IOTests.h"
#include "../../tests/SimulationTests.h"
#endif

/**
 * Parses command line arguments, stores them in the function parameters accordingly.
 *
 * @return empty string if successful, invalid flag if found one.
 */
std::string parseCmdArguments(int argc, char **argv, std::string &travelPath, std::string &algorithmPath, std::string &outputPath);

const std::string cmdFormat = "Format: ./simulator [-travel_path <path>] [-algorithm_path <algorithm path>] [-output <output path>]";

int main(int argc, char **argv) {
//    runIOTests();
//    tableResultSortTest();
//    return 0;

    if (argc != 3 && argc != 5 && argc != 7) {
        std::cerr << "Insufficient arguments supplied." << std::endl;
        std::cerr << cmdFormat << std::endl;
        return EXIT_FAILURE;
    }

    std::string travelPath = "", algorithmPath = "", outputPath = "";

    std::string invalidFlag = parseCmdArguments(argc, argv, travelPath, algorithmPath, outputPath);

    if (invalidFlag != "") {
        std::cerr << "Invalid flag received: '" << invalidFlag << "'" << std::endl;
        std::cerr << cmdFormat << std::endl;
        return EXIT_FAILURE;
    }

    bool created = createFolder(outputPath);
    if (!created) {
        std::cerr << "Couldn't initialize output directory." << std::endl;
        return EXIT_FAILURE;
    }

//    travelPath = "../input-examples/single-travel-difference";

    Simulator simulator(travelPath, algorithmPath, outputPath);

    simulator.runSimulations();

    return EXIT_SUCCESS;
}

std::string parseCmdArguments(int argc, char **argv, std::string &travelPath, std::string &algorithmPath, std::string &outputPath) {
    for (int i = 1; i < argc-1; i += 2) {
        std::string flag = argv[i];

        if (flag == "-travel_path") {
            travelPath = argv[i + 1];
        }

        else if (flag == "-algorithm_path") {
            algorithmPath = argv[i + 1];
        }

        else if (flag == "-output") {
            outputPath = argv[i + 1];
        }
        else return flag;  // error
    }

    return "";  // success
}
