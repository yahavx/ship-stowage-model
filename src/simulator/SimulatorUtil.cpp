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
#include "../interfaces/AbstractAlgorithm.h"
#include "../common/io/ObjectsReader.h"
#include "../common/io/FileReader.h"
#include "../common/utils/Definitions.h"
#include "../common/utils/UtilFunctions.h"
#include "../common/utils/Printers.h"


// region Simulation utils

StringToIntMap initPortsVisits(ShipRoute &shipRoute) {
    StringToIntMap map;

    for (PortId &port: shipRoute.getPorts()) {
        map[port] = 0;  // we may do it for the same port twice - no problem with that (one copy will be overwritten)
    }

    return map;
}

int getVisitNum(StringToIntMap &portsVisits, const PortId &portId) {
    return ++portsVisits[portId];
}

std::string getNextFileForPort(StringToStringVectorMap &cargoData, StringToIntMap &portVisits, const PortId &portId, SimulatorDataManager &manager, int isLast) {
    std::string portCode = portId;
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
    std::string filePath = manager.createCargoDataTempFilePath(portCode);
    return filePath;
}

void filterUnusedPorts(StringToStringVectorMap &map, ShipRoute &shipRoute, Errors &errors) {
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
            std::cout << "Warning: port " << currPortCode << " has cargo files but doesn't appear in the route, ignoring" << std::endl;
            errors.addError({ErrorFlag::Travel_CargoData_PortNotInRoute, currPortCode});
            toErase.push_back(currPortCode);  // we don't erase in-place because it will crash the map iterator
        }
    }

    for (std::string &port : toErase) {
        map.erase(port);
    }
}

void validateNoCargoFilesLeft(StringToStringVectorMap &map, Errors &errors) {
    for (auto &entry: map) {
        std::string portId = entry.first;
        if (!map[portId].empty()) {
            std::cout << "Warning: finished the route, but port " << portId << " have cargo files that were not used" << std::endl;
            errors.addError({ErrorFlag::Travel_CargoData_RemainingFilesAfterFinish, portId});
        }
    }
}

int extractNumberFromCargoFile(const std::string filePath) {
    std::string file = extractFilenameFromPath(filePath, true);
    file = file.substr(6, file.size() - 6);
    return strToInt(file);
}

// endregion

// region Table data manager

void initResultsTable(StringStringVector &results, StringVector &travels, StringVector &algorithmsNames) {
    // init results table
    StringVector &resultsFirstRow = results.emplace_back();

    resultsFirstRow.push_back(Simulator::s_resultsTableTitle);  // set table title

    for (auto &travel : travels) {  // first row init (column names)
        auto travelName = extractFilenameFromPath(travel);
        resultsFirstRow.push_back(travelName);
    }
    resultsFirstRow.push_back(Simulator::s_sumColumnTitle);
    resultsFirstRow.push_back(Simulator::s_errorsColumnTitle);

    for (auto &algorithmsName : algorithmsNames) {  // init a row for each algorithm
//        std::cout << "Added row for " + algorithmsName << std::endl;
        results.emplace_back();
        results.back().push_back(algorithmsName);
    }

//    std::cout << "Number of rows in results table: " << results.size() << std::endl;
}

void addSimulationResultToTable(StringStringVector &simulationResults, int totalCraneInstructions, int rowNum) {
//    std::cout << "Adding results to row " << rowNum << " in results table" << std::endl;
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
            } else {
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

    std::sort(results.begin() + 1, results.end(), [](StringVector &row1, StringVector &row2) -> bool {
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