#include <iostream>
#include "../common/io/FileReader.h"
#include "../common/io/ObjectsReader.h"
#include "../common/utils/UtilFunctions.h"
#include "Simulator.h"
#include "SimulatorUtil.h"
#include "concurrency/ThreadPoolExecutor.h"
#include "concurrency/AlgorithmTravelTaskProducer.h"

#ifndef RUNNING_ON_NOVA

#include "../../tests/IOTests.h"
#include "../../tests/SimulationTests.h"

#endif

/**
 * Parses command line arguments, stores them in the function parameters accordingly.
 *
 * @return empty string if successful, invalid flag if found one.
 */
std::string parseCmdArguments(int argc, char **argv, std::string &travelPath, std::string &algorithmPath, std::string &outputPath, std::string &numThreads);

const std::string cmdFormat = "Format: ./simulator [-travel_path <path>] [-algorithm_path <algorithm path>] [-output <output path>] [-num_threads <num>]";

int main(int argc, char **argv) {
//    runIOTests();
//    simulationTests();
//    return 0;

    if (argc != 3 && argc != 5 && argc != 7 && argc != 9) {
        std::cerr << "Incorrect number of arguments supplied: should be 2, 4 or 6 (according to the number of flags)" << std::endl;
        std::cerr << cmdFormat << std::endl;
        std::cerr << "Program is terminated." << std::endl;
        return EXIT_FAILURE;
    }

    std::string travelPath = "", algorithmPath = ".", outputPath = ".", numThreads = "1";  // default is cwd

    std::string invalidFlag = parseCmdArguments(argc, argv, travelPath, algorithmPath, outputPath, numThreads);

    if (invalidFlag != "") {
        std::cerr << "Invalid flag received: '" << invalidFlag << "'" << std::endl;
        std::cerr << cmdFormat << std::endl;
        std::cerr << "Program is terminated." << std::endl;
        return EXIT_FAILURE;
    }

    if (!isInteger(numThreads) || strToInt(numThreads) == 0) {
        std::cerr << "Received an invalid number of threads: '" << numThreads << "', should be a positive number" << std::endl;
        std::cerr << "Program is terminated." << std::endl;
    }

    bool created = createFolder(outputPath);

    if (!created) {
        std::cerr << "Couldn't create output directory: '" << outputPath << "'" << std::endl;
        std::cerr << "Program is terminated." << std::endl;
        return EXIT_FAILURE;
    }

//    numThreads = "5";
//    travelPath = "../input-examples/Travel-Roots/single-travel-difference";

    Simulator simulator(travelPath, algorithmPath, outputPath, strToInt(numThreads));

    simulator.runSimulations();

    return EXIT_SUCCESS;
}

std::string parseCmdArguments(int argc, char **argv, std::string &travelPath, std::string &algorithmPath, std::string &outputPath, std::string &numThreads) {
    for (int i = 1; i < argc - 1; i += 2) {
        std::string flag = argv[i];

        if (flag == "-travel_path") {
            travelPath = argv[i + 1];
        } else if (flag == "-algorithm_path") {
            algorithmPath = argv[i + 1];
        } else if (flag == "-output") {
            outputPath = argv[i + 1];
        } else if (flag == "-num_threads") {
            numThreads = argv[i + 1];
        } else return flag;  // error
    }

    return "";  // success
}
