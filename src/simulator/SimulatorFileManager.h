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

    SimulatorFileManager(const std::string &outputDir, const std::string &travelRootDir, const std::string &travelName, const std::string &algorithmName);

    // endregion

    // region Getters and setters

    void setTravelName(const std::string &travelName);

    void setAlgorithmName(const std::string &algorithmName);

    // endregion

    // region Paths generator

    /// Temporary folder, can store any temporary files here. Currently used for dummy cargo_data files.
    std::string tempFolder();

    /// All errors (general and simulation-specific) are stored under this directory.
    std::string errorsFolder();

    std::string shipPlanPath();

    std::string shipRoutePath();

    /// Crane instructions sub-folders of algorithm-travel pairs reside under this directory.
    std::string craneInstructionsRootFolder();

    /// Returns crane instructions folder path for a single algorithm-travel pair.
    std::string craneInstructionsSimulationFolder();

    /**
    * Creates crane instructions sub-folder for a single port visit.
    *
    * @param portId id of the port
    * @param i visit number in this port
    */
    std::string craneInstructionsOutputPath(const std::string &portId, int i);

    /**
    * Creates an empty file in case there is no available cargo_data file in a port visit.
    *
    * @param portId id of the port (not really a must, file actually can be named whatever we want, even be static).
    */
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

private:

/// Checks if a file is in cargo format: ABCDE_<num>.cargo_data.
    bool isCargoDataFileFormat(const std::string &fileName);

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
