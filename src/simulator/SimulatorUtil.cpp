//
// Created by t-yabeny on 4/19/2020.
//


#include "SimulatorUtil.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <filesystem>
#include "Simulator.h"
#include "../algorithms/stowage/IStowageAlgorithm.h"
#include "../common/Constants.h"
#include "../common/io/ObjectsReader.h"
#include "../common/io/FileReader.h"
#include "../utils/Definitions.h"
#include "../utils/UtilFunctions.h"
#include "../utils/Printers.h"


// region Simulation utils

// region sortTravelCargoData

/// Returns a list of files of a directory
StringVector getFilesFromDirectory(const std::string &directoryPath) {
    StringVector stringVector;

    for (auto &tmp_directory : std::filesystem::directory_iterator(directoryPath)) {
        std::ostringstream myObjectStream; // a stream is built
        myObjectStream << tmp_directory;  // write file to stream
        auto path = myObjectStream.str();
        stringVector.push_back(path.substr(1, path.size() - 2));  // pull result from stream, trim the "" from both sides
    }

    return stringVector;
}

/// Sorts a string vector of .cargo_files by their numbers. Names must be valid, and all belong to each port.
void sortCargoFilesByNumber(StringVector &stringVector) {
    sort(stringVector.begin(), stringVector.end(),
         [](const std::string &a, const std::string &b) -> bool {
             std::string numA = a.substr(6, a.length() - 17);  // dirty trick to extract the number
             std::string numB = b.substr(6, b.length() - 17);
             return stringToInt(numA) < stringToInt(numB);
         });
}

StringToStringVectorMap sortTravelCargoData(const std::string &directoryPath) {
    StringToStringVectorMap map;

    StringVector files = getFilesFromDirectory(directoryPath);

    for (std::string file : files) {
        std::string fileName = extractFilenameFromPath(file, false);

        if (!isCargoDataFileFormat(fileName)) {
            if (fileName == "Route" || fileName == "Plan") {
                continue;  // we except to see this, so just ignore
            }

            std::cout << "Warning: invalid file in travel folder: " << fileName << std::endl;
            continue;
        }

        std::string portId = fileName.substr(0, 5);

        if (map.find(portId) == map.end()) {  // port not encountered yet
            map[portId] = StringVector();
        }

        map[portId].push_back(fileName);  // we push in correct order because files are sorted
    }

    // now we have mapping from AAAAA -> AAAAA_17, AAAAA_2 (unsorted because 17 < 2 but alphabetically 2 < 17)
    for (auto &entry: map) {
        sortCargoFilesByNumber(entry.second);  // sort for each port separately
    }

    return map;
}
// endregion

std::optional<std::string> getNextFileForPort(StringToStringVectorMap &map, const std::string &portId) {
    if (map.find(portId) == map.end()) {  // this port does not exist
        return std::nullopt;
    }

    StringVector &filesForPort = map[portId];

    if (filesForPort.size() == 0) {  // no files remaining
        return std::nullopt;
    }

    std::string first = filesForPort[0];  // retrieve first element
    filesForPort.erase(filesForPort.begin());  // pop first element
    return first;
}

void filterUnusedPorts(StringToStringVectorMap &map, ShipRoute &shipRoute) {
    StringVector toErase;

    for (auto &entry: map) {
        std::string currPortCode = entry.first;  // get a port id

        bool found = false;  // indicates if we found this port in the route
        for (const PortId &portId : shipRoute.getPorts()) {
            if (portId.getCode() == currPortCode) {
                found = true;
                break;
            }
        }

        if (!found) {  // port is not in the route, remove it
            std::cerr << "Warning: port " << currPortCode << " has cargo files but doesn't appear in the route, ignoring" << std::endl;
            toErase.push_back(currPortCode);  // we don't erase in-place because it will crash the map iterator
        }
    }

    for (std::string &port : toErase) {
        map.erase(port);
    }
}

void filterTwiceInARowPorts(ShipRoute &shipRoute) {
    StringVector toErase;
    std::string last = "";
    std::vector<PortId> newPorts;

    for (PortId portId : shipRoute.getPorts()) {
        if (portId.getCode() == last)
            continue;
        last = portId.getCode();
        newPorts.push_back(PortId(portId));
    }
    shipRoute.setPorts(newPorts);  // we don't if we didn't change anything also, but whatever
}

std::string getCargoPath(const std::string &travel, const std::string &cargoFile) {
    return travel + "/" + cargoFile;
}

bool getInstructionsForCargo(IStowageAlgorithm &algorithm, const std::string &travel, StringToStringVectorMap &map, Port &port, bool isLast, const std::string &instructionsOutput) {
    if (isLast) {  // the last one only unloads
        algorithm.getInstructionsForCargo(Constants::s_unloadOnly + port.getId().getCode(), instructionsOutput);
        return true;
    }

    std::optional<std::string> cargoFile = getNextFileForPort(map, port.getId().getCode());  // get cargo file of current port
    if (!cargoFile.has_value()) {  // couldn't find a cargo file
        std::cout << "Warning: no cargo file for current visit, ship will only unload" << std::endl;
        algorithm.getInstructionsForCargo(Constants::s_unloadOnly + port.getId().getCode(), instructionsOutput);
        return true;
    }

    std::string cargoFilePath = getCargoPath(travel, *cargoFile);
    algorithm.getInstructionsForCargo(cargoFilePath, instructionsOutput);

    std::optional<ContainerStorage> containers = readCargoToPortFromFile(cargoFilePath);

    if (!containers.has_value()) {
        std::cout << "Critical warning: couldn't load port information" << std::endl;
        std::cout << "The ship is continuing to the next port..." << std::endl;
        return false;
    }
    port.setStorage(*containers);

    return true;
}

void validateNoCargoFilesLeft(StringToStringVectorMap &map) {
    for (auto &entry: map) {
        std::string portId = entry.first;
        if (map[portId].size() > 0) {
            std::cout << "Warning: finished the route, but port " << portId << " have cargo files that were not used"
                      << std::endl;
        }
    }
}

StringVector collectTravels(const std::string &travelPath) {
    return getFilesFromDirectory(travelPath);
}
// endregion

// region Table data manager

void initSimulationTables(StringStringVector &results, StringStringVector &errors, StringVector &travels, std::vector<IStowageAlgorithm *> &algorithms) {
    // init results table
    results.emplace_back();

    StringVector &resultsFirstRow = results.back();
    resultsFirstRow.push_back(Simulator::s_resultsTableTitle);  // set table title
    for (auto &travel : travels) {  // first row init
        auto travelName = extractFilenameFromPath(travel, false);
        resultsFirstRow.push_back(travelName);  // add travel name for each column
    }
    resultsFirstRow.push_back(Simulator::s_sumColumnTitle);

    for (auto &algorithm : algorithms) {  // rest of rows init
        results.emplace_back();
        results.back().push_back(algorithm->getAlgorithmName());
    }

    // init errors table

    errors.emplace_back();

    StringVector &errorsFirstRow = errors.back();
    errorsFirstRow.push_back(Simulator::s_errorsTableTitle);

    // we don't init the rest of the first row, because we can't know in advance the max number of errors

    for (auto &algorithm : algorithms) {
        errors.emplace_back();
        errors.back().push_back(algorithm->getAlgorithmName());
    }
}

void saveSimulationTables(const StringStringVector &results, const StringStringVector &errors, const std::string &outputDir) {
    writeFile(outputDir + "/simulation.results.csv", results);
    writeFile(outputDir + "/simulation.errors.csv", errors);
}

void addTravelResultsToTable(StringStringVector &simulationResults, StringStringVector &results, StringStringVector &errors, int rowNum) {
    // extract simulation results and errors
    StringVector &travelResults = simulationResults[0];
    StringVector &travelErrors = simulationResults[1];

    // get results and errors row in output table (to append to them)
    StringVector &resultsRow = results[rowNum];
    StringVector &errorsRow = errors[rowNum];

    // append results data (now its only one thing: number of steps)
    resultsRow.push_back(travelResults[0]);

    // append errors data
    for (auto &error : travelErrors) {
        errorsRow.push_back(error);
    }
}

void orderSimulationTables(StringStringVector &results, StringStringVector &errors) {
    // add sums to results table
    for (longUInt i = 1; i < results.size(); i++) {
        auto &rowEntry = results[i];
        int totalOps = 0;

        for (longUInt j = 1; j < rowEntry.size(); j++) {
            auto &currSum = rowEntry[j];
            if (isInteger(currSum)) {
                totalOps += stringToInt(currSum);
            }
        }
        rowEntry.push_back(intToString(totalOps));
    }

    // add error columns
    int maxErrors = 0;  // check which algorithm has the most errors
    for (auto &errorRow: errors) {
        maxErrors = std::max(maxErrors, (int) errorRow.size());
    }

    auto &firstErrorRow = errors[0];

    for (int k = 1; k < maxErrors; k++) {  // add columns, start from 1 because we already have one (the title)
        firstErrorRow.push_back(Simulator::s_errorToken + " " + intToString(k));
    }
}

void printSimulationInfo(const std::string &travel, IStowageAlgorithm *&algorithm) {
    printSeparator(1, 1);
    std::string travelName = extractFilenameFromPath(travel, true);
    std::cout << "Simulating travel " << travelName << ", using " << algorithm->getAlgorithmName() << std::endl;
    printSeparator(1, 1);

    std::cerr << "--------------------------------------\n";
    std::cerr << "Simulating travel " << travelName << ", using " << algorithm->getAlgorithmName() << std::endl << std::endl;  // print to err also to separate
}
// endregion