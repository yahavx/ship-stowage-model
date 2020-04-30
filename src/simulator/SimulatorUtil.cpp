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
#include "../algorithms/stowage/AbstractAlgorithm.h"
#include "../common/utils/Constants.h"
#include "../common/io/ObjectsReader.h"
#include "../common/io/FileReader.h"
#include "../common/utils/Definitions.h"
#include "../common/utils/UtilFunctions.h"
#include "../common/utils/Printers.h"


// region Simulation utils

std::string getNextFileForPort(StringToStringVectorMap &cargoData, StringToIntMap &portVisits, const PortId &portId, const std::string &outputDir, int isLast) {
    std::string portCOde = portId.getCode();
    StringVector &filesForPort = cargoData[portCOde];

    if (filesForPort.size() > 0) {

        std::string portCargoFile = filesForPort[0];  // retrieve cargo_data with smallest index

        if (extractNumberFromCargoFile(portCargoFile) == portVisits[portCOde]) {  // it matches the port visit number, pop it and return
            filesForPort.erase(filesForPort.begin());
            return portCargoFile;
        }
    }

    if (!isLast)  // TODO: no file for current visit. maybe need to generate an error.
        std::cout << "";

    // there is no matching cargo_data file, so we will generate an empty one
    std::string filePath = getCargoDataTempFilePath(outputDir, portCOde);
    createEmptyFile(filePath);
    return filePath;
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

// region Cargo data manager  // TODO: move some functions from above to this region maybe

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

    for (std::string& file : files) {
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

StringToIntMap initPortsVisits(ShipRoute &shipRoute) {
    StringToIntMap map;

    for (PortId& port: shipRoute.getPorts()) {
        map[port.getCode()] = 0;  // we may do it for the same port twice - no problem with that (one copy will be overwritten)
    }

    return map;
}

int getVisitNum(StringToIntMap &portsVisits, const PortId &portId) {
    return ++portsVisits[portId.getCode()];
}
// endregion

// region Table data manager

void initSimulationTables(StringStringVector &results, StringStringVector &errors, StringVector &travels, std::vector<AbstractAlgorithm *> &algorithms) {
    // init results table
    results.emplace_back();

    StringVector &resultsFirstRow = results.back();
    resultsFirstRow.push_back(Simulator::s_resultsTableTitle);  // set table title
    for (auto &travel : travels) {  // first row init
        auto travelName = extractFilenameFromPath(travel, false);
        resultsFirstRow.push_back(travelName);  // add travel name for each column
    }
    resultsFirstRow.push_back(Simulator::s_sumColumnTitle);
    resultsFirstRow.push_back(Simulator::s_errorsColumnTitle);

    for (auto &algorithm : algorithms) {  // rest of rows init
        results.emplace_back();
        results.back().push_back(algorithm->getAlgorithmName());
    }

    // init errors table

    errors.emplace_back();  // first row: table title
    errors.back().push_back(Simulator::s_errorsTableTitle);

    errors.emplace_back();  // second row: general errors
    errors.back().push_back(Simulator::s_generalErrorsRowTitle);


    for (auto &algorithm : algorithms) {  // rows 3 - n: algorithm errors
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
    StringVector &errorsRow = errors[rowNum + 1];  // in the errors, second row is reserved for general errors

    // append results data
    resultsRow.push_back(travelResults[0]);  // number of steps

    // append errors data
    for (auto &error : travelErrors) {
        errorsRow.push_back(error);
    }
}

void orderSimulationTables(StringStringVector &results, StringStringVector &errors) {
    // add sums to results table
    for (longUInt i = 1; i < results.size(); i++) {  // for each algorithm
        auto &rowEntry = results[i];
        int totalOps = 0;

        for (longUInt j = 1; j < rowEntry.size(); j++) {  // sum his operations from all the travels
            auto &currSum = rowEntry[j];
            if (isInteger(currSum)) {
                totalOps += stringToInt(currSum);
            }
        }
        rowEntry.push_back(intToString(totalOps));  // push the sum
        rowEntry.push_back("0");  // push number of errors TODO: actual number of errors
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

void printSimulationInfo(const std::string &travel, AbstractAlgorithm *&algorithm) {
    printSeparator(1, 1);
    std::string travelName = extractFilenameFromPath(travel, true);
    std::cout << "Simulating travel " << travelName << ", using " << algorithm->getAlgorithmName() << std::endl;
    printSeparator(1, 1);

    std::cerr << "--------------------------------------\n";
    std::cerr << "Simulating travel " << travelName << ", using " << algorithm->getAlgorithmName() << std::endl << std::endl;  // print to err also to separate


}

void addGeneralError(StringStringVector &errors, const std::string &error) {
    auto& generalErrorsRow = errors[1];  // assuming the general errors are the second row
    generalErrorsRow.push_back(error);
}
// endregion

// region Path generation

std::string getShipPlanPath(const std::string &travel) {
    return travel + "/Plan";
}

std::string getShipRoutePath(const std::string &travel) {
    return travel + "/Route";
}

std::string getCraneInstructionsRootFolder(const std::string &outputDir) {
    return outputDir + "/crane_instructions";
}

std::string getCraneInstructionsSimulationFolder(const std::string &outputDir, const std::string &algorithmName, const std::string &travelName) {
    return getCraneInstructionsRootFolder(outputDir) + "/" + algorithmName + "_" + travelName + "_crane_instructions";
}

std::string getCraneInstructionsOutputFilePath(const std::string &craneOutputDir, const PortId &portId, int i) {
    return craneOutputDir + "/" + portId.getCode() + "_" + intToString(i) + ".crane_instructions";
}

std::string getTempFolderPath(const std::string &outputDir) {
    return outputDir + "/" + "temp";
}

std::string getCargoDataTempFilePath(const std::string &outputDir, const std::string &portId) {
    return getTempFolderPath(outputDir) + "/" + portId + "_0.cargo_data";
}

std::string getCargoPath(const std::string &travel, const std::string &cargoFile) {
    return travel + "/" + cargoFile;
}
// endregion

int extractNumberFromCargoFile(const std::string filePath) {
    std::string file = extractFilenameFromPath(filePath, true);
    file = file.substr(6, file.size() - 6);
    return stringToInt(file);
}