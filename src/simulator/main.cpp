#include <iostream>
#include "../common/io/FileReader.h"
#include "../common/io/ObjectsReader.h"
#include "../common/utils/UtilFunctions.h"
#include "Simulator.h"
#include "SimulatorUtil.h"
#include "ThreadPoolExecuter.h"
#include "SimpleTaskProducer.h"

#ifndef RUNNING_ON_NOVA

#include "../../tests/IOTests.h"
#include "../../tests/SimulationTests.h"

#endif

/**
 * Parses command line arguments, stores them in the function parameters accordingly.
 *
 * @return empty string if successful, invalid flag if found one.
 */
std::string parseCmdArguments(int argc, char **argv,
                              std::string &travelPath, std::string &algorithmPath, std::string &outputPath,
                              int &num_threads);

const std::string cmdFormat = "Format: ./simulator [-travel_path <path>] [-algorithm_path <algorithm path>] [-output <output path>]";

//int main() {
//    ThreadPoolExecuter executer1 {
//            SimpleTasksProducer{NumTasks{8}, IterationsPerTask{200}},
//            NumThreads{5}
//    };
//    std::cout << "first cycle started" << std::endl;
//    executer1.start();
//    executer1.wait_till_finish();
//    std::cout << "first cycle finished" << std::endl;
//
//    ThreadPoolExecuter executer2 {
//            SimpleTasksProducer{NumTasks{5}, IterationsPerTask{500}},
//            NumThreads{2}
//    };
//    std::cout << "second cycle started" << std::endl;
//    executer2.start();
//    using namespace std::chrono_literals;
//    std::this_thread::sleep_for(10ms);
//    executer2.stop_gracefully();
//    std::cout << "second cycle stopped" << std::endl;
//}

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

    std::string travelPath = "", algorithmPath = ".", outputPath = ".";  // default is cwd
    int num_threads = 1;

    std::string invalidFlag = parseCmdArguments(argc, argv, travelPath, algorithmPath, outputPath, num_threads);

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

    Simulator simulator(travelPath, algorithmPath, outputPath, num_threads);

    simulator.runSimulations();

    return EXIT_SUCCESS;
}

std::string parseCmdArguments(int argc, char **argv,
                              std::string &travelPath, std::string &algorithmPath, std::string &outputPath,
                              int &num_threads) {
    for (int i = 1; i < argc - 1; i += 2) {
        std::string flag = argv[i];

        if (flag == "-travel_path") {
            travelPath = argv[i + 1];
        } else if (flag == "-algorithm_path") {
            algorithmPath = argv[i + 1];
        } else if (flag == "-output") {
            outputPath = argv[i + 1];
        } else if (flag == "-num_threads") {
            num_threads = atoi(argv[i+1]);
        } else return flag;  // error
    }

    return "";  // success
}
