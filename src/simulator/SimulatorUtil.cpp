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

std::string getNextFileForPort(StringToStringVectorMap &cargoData, StringToIntMap &portVisits, const PortId &portId, SimulatorDataManager &manager, int isLast) {
    std::string portCode = portId.getCode();
    StringVector &filesForPort = cargoData[portCode];

    if (!filesForPort.empty()) {
        std::string portCargoFile = filesForPort[0];  // retrieve cargo_data with smallest index

        if (extractNumberFromCargoFile(portCargoFile) == portVisits[portCode]) {  // it matches the port visit number, pop it and return
            filesForPort.erase(filesForPort.begin());
            return portCargoFile;
        }
    }

    if (!isLast)  // TODO: no file for current visit. maybe need to generate an error.
        std::cout << "";

    // there is no matching cargo_data file, so we will generate an empty one
    std::string filePath = manager.cargoDataTempFilePath(portCode);
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

bool isTravelValid(SimulatorDataManager &manager, Errors &errors) {
    if (!isDirectoryExists(manager.travelFolder())) {
        errors.addError({ErrorFlag::Travel_InvalidDirectory, manager.travelName});
        return false;
    }

    Errors tempErrors;

    readShipPlanFromFile(manager.shipPlanPath(), tempErrors);
    if (tempErrors.hasFatalError()) {
        errors.addError({ErrorFlag::Travel_InvalidInput, manager.travelName, "Ship Plan"});
        return false;
    }

    readShipRouteFromFile(manager.shipRoutePath(), tempErrors);
    if (tempErrors.hasFatalError()) {
        errors.addError({ErrorFlag::Travel_InvalidInput, manager.travelName, "Ship Route"});
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
             return strToInt(numA) < strToInt(numB);
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

void initResultsTable(StringStringVector &results, StringVector &travels, std::vector<std::shared_ptr<AbstractAlgorithm>> &algorithms) {
    // init results table
    StringVector &resultsFirstRow = results.emplace_back();

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

void addSimulationResultToTable(StringStringVector &simulationResults, int totalCraneInstructions, int rowNum) {
    simulationResults[rowNum].push_back(intToStr(totalCraneInstructions));
}

void finalizeResultsTable(StringStringVector &results) {
    for (longUInt i = 1; i < results.size(); i++) {  // for each algorithm
        auto &rowEntry = results[i];
        int totalOps = 0;
        int errors = 0;

        for (longUInt j = 1; j < rowEntry.size(); j++) {  // sum his operations/errors from all the travels
            int ops = strToInt(rowEntry[j]);

            if (ops == -1) {
                errors++;
            }
            else {
                totalOps += ops;
            }
        }

        // push result
        rowEntry.push_back(intToStr(totalOps));
        rowEntry.push_back(intToStr(errors));
    }

    sortResultsTable(results);
}

void sortResultsTable(StringStringVector &results) {
    if (results.size() <= 2)  // nothing to sort
        return;

    std::sort(results.begin() + 1, results.end(), [](StringVector& row1, StringVector& row2) -> bool {
        int n = row1.size();  // should be same size for both
        int errors1 = strToInt(row1[n - 1]), steps1 = strToInt(row1[n - 2]);
        int errors2 = strToInt(row2[n - 1]), steps2 = strToInt(row2[n - 2]);

        if (errors1 == errors2) {
            return steps1 < steps2;
        }
        return errors1 < errors2;
    });
}

// endregion

int extractNumberFromCargoFile(const std::string filePath) {
    std::string file = extractFilenameFromPath(filePath, true);
    file = file.substr(6, file.size() - 6);
    return strToInt(file);
}