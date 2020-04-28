#include <iostream>
#include "src/common/io/FileReader.h"
#include "src/common/io/ObjectsReader.h"
#include "src/utils/UtilFunctions.h"
#include "src/simulator/Simulator.h"

void parseCmdArguments(int argc, char **argv, std::string &travelPath, std::string &algorithmPath, std::string &outputPath);

int main(int argc, char **argv) {
    if (argc != 3 && argc != 5 && argc != 7) {
        std::cerr << "Insufficient arguments supplied. Please read the README for usage instructions. Program is terminated." << std::endl;
        return 1;
    }

    std::string travelPath = "", algorithmPath = "", outputPath = "";

    parseCmdArguments(argc, argv, travelPath, algorithmPath, outputPath);

    if (!isDirectoryExists(outputPath)) {  // the output directory doesn't exist
        bool created = createFolder(outputPath);  // try to create it
        if (!created) {
            std::cerr << "Couldn't initialize output directory. Please read the README for usage instructions. Program is terminated." << std::endl;
            return 1;
        }
    }

    // Direct log prints to log files
    std::string outputLogFile = outputPath + "/output.txt";
    std::string errorLogFile =  outputPath + "/error.txt";
    freopen(outputLogFile.c_str(), "w", stdout);  // TODO
    freopen(errorLogFile.c_str(), "w", stderr);

    StringVector travels;

    for (int i = 2; i < argc; i++) {
        travels.push_back(argv[i]);
    }

    Simulator simulator(outputPath);

    simulator.runSimulations(travels);

    return 0;
}

void parseCmdArguments(int argc, char **argv, std::string &travelPath, std::string &algorithmPath, std::string &outputPath) {
    for (int i = 1; i < argc; i += 2) {
        std::string flag = argv[i];

        if (flag == "-travel_path") {
            travelPath = argv[i+1];
        }

        if (flag == "-algorithm_path") {
            algorithmPath = argv[i+1];
        }

        if (flag == "-output") {
            outputPath = argv[i+1];
        }

    }
}