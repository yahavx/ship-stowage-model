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
std::string parseCmdArguments(int argc, char **argv, std::string &travelPath, std::string &algorithmPath, std::string &outputPath, std::string &threadsNum);

const std::string cmdFormat = "Format: ./simulator [-travel_path <path>] [-algorithm_path <algorithm path>] [-output <output path>] [-num_threads <number>]";

int main(int argc, char **argv) {
//    runIOTests();
//    simulationTests();
//    return 0;

    if (!(argc >= 3 && argc <= 9 && argc % 2 == 1)) {  // odd number, between 3 to 9
        std::cerr << "Incorrect number of arguments supplied: should be 2, 4, 6 or 8 (according to the number of flags)" << std::endl;
        std::cerr << cmdFormat << std::endl;
        std::cerr << "Program is terminated." << std::endl;
        return EXIT_FAILURE;
    }

    std::string travelPath = "", algorithmPath = ".", outputPath = ".", threadsNum = "1";  // default is cwd

    std::string invalidFlag = parseCmdArguments(argc, argv, travelPath, algorithmPath, outputPath, threadsNum);

    if (invalidFlag != "") {
        std::cerr << "Invalid flag received: '" << invalidFlag << "'" << std::endl;
        std::cerr << cmdFormat << std::endl;
        std::cerr << "Program is terminated." << std::endl;
        return EXIT_FAILURE;
    }

    bool created = createFolder(outputPath);

    if (!created) {
        std::cerr << "Couldn't create output directory: '" << outputPath << "'" << std::endl;
        std::cerr << "Program is terminated." << std::endl;
        return EXIT_FAILURE;
    }

    if (!isInteger(threadsNum) || strToInt(threadsNum) == 0) {
        std::cerr << "Received an invalid number of threads: '" << threadsNum << "', should be a positive number" << std::endl;
        std::cerr << "Program is terminated." << std::endl;
    }


//    travelPath = "../input-examples/Travel-Roots/single-travel";

    Simulator simulator(travelPath, algorithmPath, outputPath);

    simulator.runSimulations(strToInt(threadsNum));

    return EXIT_SUCCESS;
}

std::string parseCmdArguments(int argc, char **argv, std::string &travelPath, std::string &algorithmPath, std::string &outputPath, std::string &threadsNum) {
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
        else if (flag == "-num_threads") {
            threadsNum = argv[i + 1];
        }

        else return flag;  // error
    }

    return "";  // success
}
