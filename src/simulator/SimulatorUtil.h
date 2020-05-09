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
#include "SimulatorDataManager.h"

// region Simulation utils

/// Inits a map from each port id to zero (will be incremented each time we visit this port)
StringToIntMap initPortsVisits(ShipRoute &shipRoute);

/// Returns the visit number at the specified port. Port must exist in portsVisits - otherwise might throw an exception.
int getVisitNum(StringToIntMap &portsVisits, const PortId &portId);

/// Returns the next .cargo_data file of the port with portId. Generates one if needed (inside tempDir).
std::string getNextFileForPort(StringToStringVectorMap &cargoData, StringToIntMap &portVisits, const PortId &portId, SimulatorDataManager &manager, int isLast);

/// Removes from map ports that doesn't appear in the route.
void filterUnusedPorts(StringToStringVectorMap &map, ShipRoute &shipRoute, Errors &errors);

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
