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
    int tableHeight = algorithmsNames.size() + 1;
    int tableWidth = travels.size() + 3;

    results = StringStringVector(tableHeight, StringVector(tableWidth));

    auto& firstRow = results[0];

    firstRow[0] = Simulator::s_resultsTableTitle;  // Set table title

    for (longUInt i = 0; i < travels.size(); i++) {  // First row init (column names)
        auto travelName = extractFilenameFromPath(travels[i]);
        firstRow[i + 1] = travelName;
    }

    firstRow[travels.size() + 1] = Simulator::s_sumColumnTitle;
    firstRow[travels.size() + 2] = Simulator::s_errorsColumnTitle;

    for (longUInt i = 0; i <algorithmsNames.size(); i++) {  // Init a row for each algorithm
        results[i + 1][0] = algorithmsNames[i];
    }
}

void addSimulationResultToTable(StringStringVector &simulationResults, int totalCraneInstructions, int travelNum, int algorithmNum) {
    simulationResults[algorithmNum + 1][travelNum + 1] = intToStr(totalCraneInstructions);
}

void finalizeResultsTable(StringStringVector &results) {
    longUInt rowSize = results[0].size();

    for (longUInt i = 1; i < results.size(); i++) {  // For each algorithm
        auto &rowEntry = results[i];
        int totalOps = 0;
        int errors = 0;

        for (longUInt j = 1; j < rowSize - 2; j++) {  // Sum his operations/errors from all the travels
            int ops = strToInt(rowEntry[j]);

            if (ops == -1) {
                errors++;
            } else {
                totalOps += ops;
            }
        }

        // push result
        rowEntry[rowSize - 2] = intToStr(totalOps);
        rowEntry[rowSize - 1] = intToStr(errors);
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