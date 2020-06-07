//
// Created by t-yabeny on 4/19/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATORUTIL_H
#define SHIP_STOWAGE_MODEL_SIMULATORUTIL_H

#include <memory>
#include "../common/utils/Definitions.h"
#include "../common/data_objects/ShipRoute.h"
#include "../interfaces/AbstractAlgorithm.h"
#include "../common/utils/Errors.h"
#include "SimulatorFileManager.h"

// region Simulation utils

/// Checks if there are any remaining ports in the map, which still have files. Prints warning if yes.
void validateNoCargoFilesLeft(StringToStringVectorMap &map, Errors &errors);

/**
 * Extract the number from a cargo_data file (AAAAA_5.cargo_data -> 5)
 * @param filePath full path to a cargo data file
 */
int extractNumberFromCargoFile(const std::string filePath);

// endregion

// region Table data manager

/// Inits results and errors table, results and errors should be empty.
void initResultsTable(StringStringVector &results, StringVector &travels, StringVector &algorithmsNames);

/// Add travel results of a single simulation to a table.
void addSimulationResultToTable(StringStringVector &simulationResults, int totalCraneInstructions, int rowNum);

/// Finalize the results table: add all sums
void finalizeResultsTable(StringStringVector &results);

/// Sorts the results table by the needed order. Assuming it is finished.
void sortResultsTable(StringStringVector &results);

// endregion

#endif //SHIP_STOWAGE_MODEL_SIMULATORUTIL_H
