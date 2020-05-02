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

std::vector<ErrorFlag> SU_errorGarbageCollector;
ErrorVector SU_errorGarbageCollector2;

// region Simulation utils

std::string getNextFileForPort(StringToStringVectorMap &cargoData, StringToIntMap &portVisits, const PortId &portId, const std::string &outputDir, int isLast) {
    std::string portCOde = portId.getCode();
    StringVector &filesForPort = cargoData[portCOde];

    if (!filesForPort.empty()) {
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

StringVector collectTravels(const std::string &travelPath, ErrorVector &errors) {
    if (travelPath.empty()) {  // no travel path supplied
        errors.push_back({"Initialization", ErrorFlag::SimulationInit_InvalidTravelPath});
    }
    StringVector files = getFilesFromDirectory(travelPath);
    if (files.empty()) {
        errors.push_back({"Initialization", ErrorFlag::SimulationInit_InvalidTravelPath});
    }

    return files;
}

bool isTravelValid(const std::string &travelDirectory, ErrorVector &errors) {
    std::string travelName = extractFilenameFromPath(travelDirectory);

    if (!isDirectoryExists(travelDirectory)) {
        errors.push_back({travelName, ErrorFlag::Travel_InvalidDirectory});
        return false;
    }

    std::vector<ErrorFlag> planErrors, routeErrors;

    readShipPlanFromFile(getShipPlanPath(travelDirectory), planErrors);
    readShipRouteFromFile(getShipRoutePath(travelDirectory), routeErrors);

    if (containsFatalError(planErrors) || containsFatalError(routeErrors)) {
        errors.push_back({travelName, ErrorFlag::Travel_InvalidInput});
        return false;
    }

    return true;
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

    for (std::string &file : files) {
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

    for (PortId &port: shipRoute.getPorts()) {
        map[port.getCode()] = 0;  // we may do it for the same port twice - no problem with that (one copy will be overwritten)
    }

    return map;
}

int getVisitNum(StringToIntMap &portsVisits, const PortId &portId) {
    return ++portsVisits[portId.getCode()];
}
// endregion

// region Table data manager

void initResultsTable(StringStringVector &results, StringVector &travels, std::vector<AbstractAlgorithm *> &algorithms) {
    // init results table
    results.emplace_back();

    StringVector &resultsFirstRow = results.back();
    resultsFirstRow.push_back(Simulator::s_resultsTableTitle);  // set table title

    for (auto &travel : travels) {  // first row init (column names)
        auto travelName = extractFilenameFromPath(travel, false);
        resultsFirstRow.push_back(travelName);
    }
    resultsFirstRow.push_back(Simulator::s_sumColumnTitle);
    resultsFirstRow.push_back(Simulator::s_errorsColumnTitle);

    for (auto &algorithm : algorithms) {  // init a row for each algorithm
        results.emplace_back();
        results.back().push_back(algorithm->getAlgorithmName());
    }
}

void saveSimulationTables(const std::string &outputDir, const StringStringVector &results, const ErrorVector &errors) {
    writeFile(outputDir + "/simulation.results.csv", results);
    if (errors.size() > 0) {
        saveErrorFile(outputDir, Simulator::s_generalErrorsTableName, errors);
    }
}

void saveErrorFile(const std::string outputDir, const std::string &fileName, const ErrorVector &errors) {
    StringVector errorMessages = errorsVectorToString(errors);

    std::string filePath = getErrorsFolderPath(outputDir) + "/" + fileName;
    writeFile(filePath, errorMessages);
}

void addSimulationResultToTable(StringStringVector &simulationResults, StringStringVector &results, int rowNum) {
    // extract simulation results and errors
    StringVector &travelResults = simulationResults[0];

    // get results and errors row in output table (to append to them)
    StringVector &resultsRow = results[rowNum];

    // append results data
    resultsRow.push_back(travelResults[0]);  // number of steps

}

void finalizeResultsTable(StringStringVector &results) {
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
}

void printSimulationInfo(const std::string &travel, AbstractAlgorithm *&algorithm) {
    printSeparator(1, 1);
    std::string travelName = extractFilenameFromPath(travel, true);
    std::cout << "Simulating travel " << travelName << ", using " << algorithm->getAlgorithmName() << std::endl;
    printSeparator(1, 1);

    std::cerr << "--------------------------------------\n";
    std::cerr << "Simulating travel " << travelName << ", using " << algorithm->getAlgorithmName() << std::endl << std::endl;  // print to err also to separate


}

void initOutputFolders(const std::string &outputDir, ErrorVector &errors) {
    bool res = createFolder(getCraneInstructionsRootFolder(outputDir));
    bool res2 = createFolder(getTempFolderPath(outputDir));
    bool res3 = createFolder(getErrorsFolderPath(outputDir));
    if (!(res && res2 && res3)) {
        errors.push_back({"Initialization", ErrorFlag::SimulationInit_OutputDirectoriesCreationFailed});
    }
}

void cleanOutputFolders(const std::string &outputDir, ErrorVector &errors) {
    bool res = removeFolder(getTempFolderPath(outputDir));

    std::string errorsFolder = getErrorsFolderPath(outputDir);

    bool res2;
    if (isFolderEmpty(errorsFolder)) {
        res2 = removeFolder(getErrorsFolderPath(outputDir));
    }

    if (!(res && res2)) {
        errors.push_back({"Cleanup", ErrorFlag::SimulationInit_OutputDirectoriesCreationFailed});
    }
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

std::string getErrorsFolderPath(const std::string &outputDir) {
    return outputDir + "/" + "errors";
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