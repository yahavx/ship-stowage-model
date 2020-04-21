//
// Created by t-yabeny on 4/19/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATORUTIL_H
#define SHIP_STOWAGE_MODEL_SIMULATORUTIL_H

#include "../utils/Definitions.h"
#include "../data_objects/ShipRoute.h"
#include "../algorithms/stowage/IStowageAlgorithm.h"

// region Simulation utils

/**
 * Receives a travel directory and sorts the .cargo_files for each port.
 * @return a map from each portID to a string vector of his .cargo_files, ordered from small to big.
 */
StringToStringVectorMap sortTravelCargoData(const std::string &directoryPath);

/// Returns the next .cargo_data file of the port with portId. Returns null if this port hasn't any cargo_data file remaining.
std::optional<std::string> getNextFileForPort(StringToStringVectorMap &map, const std::string &portId);

/// Removes from map ports that doesn't appear in the route.
void filterUnusedPorts(StringToStringVectorMap &map, ShipRoute &shipRoute);

/// Removes from route ports that appears twice in a row.
void filterTwiceInARowPorts(ShipRoute &shipRoute);

/**
 * Triggers the getInstructionsForCargo of the algorithm, adds storage to the port (of simulator) if needed.
 * @return true if succeed.
 */
bool getInstructionsForCargo(IStowageAlgorithm &algorithm, const std::string &travel, StringToStringVectorMap &map, Port &port, bool isLast);

/// Checks if there are any remaining ports in the map, which still have files. Prints warning if yes.
void validateNoCargoFilesLeft(StringToStringVectorMap &map);
// endregion

// region Table data manager

/// Inits results and errors table, results and errors should be empty.
void initSimulationTables(StringStringVector &results, StringStringVector &errors, StringVector &travels, std::vector<IStowageAlgorithm*> &algorithms);

/// Saves simulation tables. Saving location is constant and defined inside.
void saveSimulationTables(const StringStringVector &results, const StringStringVector &errors, const std::string &outputDir);

/// Add travel results of a single simulation to a table.
void addTravelResultsToTable(StringStringVector &simulationResults, StringStringVector &results, StringStringVector &errors, int rowNum);

/// Finalize the table: add sums to results table, add missing columns at errors, etc.
void orderSimulationTables(StringStringVector &results, StringStringVector &errors);

/// Prints simulation starting messages.
void printSimulationInfo(const std::string &travel, IStowageAlgorithm *&algorithm);
// endregion

#endif //SHIP_STOWAGE_MODEL_SIMULATORUTIL_H
