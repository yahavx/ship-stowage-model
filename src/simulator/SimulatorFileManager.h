//
// Created by t-yabeny on 5/2/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATORFILEMANAGER_H
#define SHIP_STOWAGE_MODEL_SIMULATORFILEMANAGER_H

#include <string>
#include "../common/utils/Errors.h"


/// Handles all path generation and work against files.
class SimulatorFileManager {
private:
    std::string shipPlanFilepath;
    std::string shipRouteFilepath;

public:
    std::string outputDir;
    std::string travelRootDir;
    std::string travelName;
    std::string algorithmName;

    // region Constructor

    SimulatorFileManager(const std::string &outputDir, const std::string &travelRootDir);

    // endregion

    // region Getters and setters

    void setTravelName(const std::string &travelName);

    void setAlgorithmName(const std::string &algorithmName);

    // endregion

    // region Paths generator

    std::string tempFolder();

    std::string errorsFolder();

    std::string shipPlanPath();

    std::string shipRoutePath();

    std::string craneInstructionsRootFolder();

    /// Crane instructions folder for a single algorithm-travel pair.
    std::string craneInstructionsSimulationFolder();

    /// Single crane instructions file.
    std::string craneInstructionsOutputPath(const std::string &portId, int i);

    /// Creates temporary cargo data file (for a port visit with no file).
    std::string createCargoDataTempFilePath(const std::string &portId);

    std::string cargoFilePath(const std::string &cargoFileName);

    std::string travelFolder();

    // endregion

    // region Functions

private:
    bool isTravelValid(Errors &errors);

public:
    /// Returns a list of legal travels inside travelPath.
    StringVector collectLegalTravels(Errors &errors);

    /**
 * Receives a travel directory and sorts the .cargo_files for each port.
 * @return a map from each portID to a string vector of his .cargo_files, ordered from small to big.
 */
    StringToStringVectorMap getCargoDataFiles(Errors &errors);

    // endregion

    // region Files IO
private:
    void saveErrorsFile(const std::string &fileName, const Errors &errorsToWrite);

public:
    void saveGeneralErrors(const Errors &errorsToWrite);  // errors to be saved - not to be collected

    /// Saves errors of a single simulation.
    void saveSimulationErrors(const Errors &errorsToWrite);

    void saveSimulationResults(const StringStringVector &results);

    /// Creates a crane folder for a single simulation run (must call setTravelInfo before).
    void createTravelCraneFolder();

    /// Create all output folders needed for runSimulations.
    void createOutputFolders(Errors &errors);

    /// Remove unneeded output folders (temp, and errors if not generated).
    void cleanOutputFolders();

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_SIMULATORFILEMANAGER_H
