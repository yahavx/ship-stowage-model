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

    resultsFirstRow.push_back(Simulator::s_resultsTableTitle);  // Set table title

    for (auto &travel : travels) {  // First row init (column names)
        auto travelName = extractFilenameFromPath(travel);
        resultsFirstRow.push_back(travelName);
    }
    resultsFirstRow.push_back(Simulator::s_sumColumnTitle);
    resultsFirstRow.push_back(Simulator::s_errorsColumnTitle);

    for (auto &algorithmsName : algorithmsNames) {  // Init a row for each algorithm
        results.emplace_back();
        results.back().push_back(algorithmsName);
    }
}

void addSimulationResultToTable(StringStringVector &simulationResults, int totalCraneInstructions, int rowNum) {
    simulationResults[rowNum].push_back(intToStr(totalCraneInstructions));  // TODO: make it less ugly..
}

void finalizeResultsTable(StringStringVector &results) {
    for (longUInt i = 1; i < results.size(); i++) {  // For each algorithm
        auto &rowEntry = results[i];
        int totalOps = 0;
        int errors = 0;

        for (longUInt j = 1; j < rowEntry.size(); j++) {  // Sum his operations/errors from all the travels
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
    if (results.size() <= 2)  // Nothing to sort
        return;

    std::sort(results.begin() + 1, results.end(), [](StringVector &row1, StringVector &row2) -> bool {
        int n = row1.size();  // Should be same size for both
        int errors1 = strToInt(row1[n - 1]), steps1 = strToInt(row1[n - 2]);
        int errors2 = strToInt(row2[n - 1]), steps2 = strToInt(row2[n - 2]);

        // Sort by number of errors, break ties with number of steps
        if (errors1 == errors2) {
            return steps1 < steps2;
        }
        return errors1 < errors2;
    });
}

// endregion