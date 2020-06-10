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

/// Inits results table filled with placeholders
void initResultsTableWithPlaceholders(StringStringVector &results, StringVector &travels, StringVector &algorithmsNames);

/// Updates travel results of a single simulation at a given position in a table.
void updateSimulationResultAtPosition(StringStringVector &simulationResults, int totalCraneInstructions, int algorithmNum, int travelNum);

/// Finalize the results table: add all sums
void finalizeResultsTable(StringStringVector &results);

/// Sorts the results table by the needed order. Assuming it is finished.
void sortResultsTable(StringStringVector &results);

// endregion

#endif //SHIP_STOWAGE_MODEL_SIMULATORUTIL_H
