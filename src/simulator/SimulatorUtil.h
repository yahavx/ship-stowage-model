//
// Created by t-yabeny on 4/19/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATORUTIL_H
#define SHIP_STOWAGE_MODEL_SIMULATORUTIL_H

#include "../common/utils/Definitions.h"
#include "../common/data_objects/ShipRoute.h"
#include "../algorithms/stowage/AbstractAlgorithm.h"
#include "../common/utils/Errors.h"

// region Simulation utils

/**
 * Receives a travel directory and sorts the .cargo_files for each port.
 * @return a map from each portID to a string vector of his .cargo_files, ordered from small to big.
 */
StringToStringVectorMap sortTravelCargoData(const std::string &directoryPath);

/// Inits a map from each port id to zero (will be incremented each time we visit this port)
StringToIntMap initPortsVisits(ShipRoute &shipRoute);

/// Returns the visit number at the specified port. Port must exist in portsVisits - otherwise might throw an exception.
int getVisitNum(StringToIntMap &portsVisits, const PortId &portId);

/// Returns the next .cargo_data file of the port with portId. Generates one if needed (inside tempDir).
std::string getNextFileForPort(StringToStringVectorMap &cargoData, StringToIntMap &portVisits, const PortId &portId, const std::string &tempDir, int isLast);

/// Removes from map ports that doesn't appear in the route.
void filterUnusedPorts(StringToStringVectorMap &map, ShipRoute &shipRoute);

/// Checks if there are any remaining ports in the map, which still have files. Prints warning if yes.
void validateNoCargoFilesLeft(StringToStringVectorMap &map);

/// Returns a list of travels (i.e. directories) inside travelPath.
StringVector collectTravels(const std::string &travelPath, Errors &errors);

/// Returns true if travel can be used (i.e. ShipRoute and ShipPlan doesn't contain a fatal error).
bool isTravelValid(const std::string &travelDirectory, Errors &errors = Errors::garbageCollector);
// endregion

// region Table data manager

/// Inits results and errors table, results and errors should be empty.
void initResultsTable(StringStringVector &results, StringVector &travels, std::vector<AbstractAlgorithm*> &algorithms);

/// Saves simulation tables.
void saveSimulationTables(const std::string &outputDir, const StringStringVector &results, const Errors &errors);

/// Saves an error file. Only filename should be supplied (not full path), it will be saved under the errors directory.
void saveErrorFile(const std::string outputDir, const std::string &fileName, const Errors &errors);

/// Add travel results of a single simulation to a table.
void addSimulationResultToTable(StringStringVector &simulationResults, StringStringVector &results, int rowNum);

/// Finalize the results table: add all sums, and sort as needed.
void finalizeResultsTable(StringStringVector &results);

/// Prints simulation starting messages.
void printSimulationInfo(const std::string &travel, AbstractAlgorithm *&algorithm);

/// Create all output folders needed for runSimulations. Adds error to errors if failed (for at least 1 folder).
void initOutputFolders(const std::string &outputDir, Errors &errors);

/// Remove unneeded output folders (temporaries, errors if not generated).
void cleanOutputFolders(const std::string &outputDir, Errors &errors = Errors::garbageCollector);
// endregion

// region Path generation

std::string getShipPlanPath(const std::string &travel);

std::string getShipRoutePath(const std::string &travel);

std::string getCraneInstructionsRootFolder(const std::string &travel);

std::string getCraneInstructionsSimulationFolder(const std::string &outputDir, const std::string &algorithmName, const std::string &travelName);

std::string getCraneInstructionsOutputFilePath(const std::string &craneOutputDir, const PortId &portId, int i);

std::string getTempFolderPath(const std::string &outputDir);

std::string getErrorsFolderPath(const std::string &outputDir);

std::string getCargoDataTempFilePath(const std::string &outputDir, const std::string &portId);

std::string getCargoPath(const std::string &travel, const std::string &cargoFile);
// endregion

/**
 * Extract the number from a cargo_data file (AAAAA_5.cargo_data -> 5)
 * @param filePath full path to a cargo data file
 */
int extractNumberFromCargoFile(const std::string filePath);

#endif //SHIP_STOWAGE_MODEL_SIMULATORUTIL_H
