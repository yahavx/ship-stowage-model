//
// Created by t-yabeny on 5/2/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATORDATAMANAGER_H
#define SHIP_STOWAGE_MODEL_SIMULATORDATAMANAGER_H


#include <string>
#include "../common/utils/Errors.h"

/// Handles all path generation and work against files.
class SimulatorDataManager {
public:

    std::string outputDir;
    std::string travelRootDir;

    std::string travelName;
    std::string algorithmName;

public:

    // region Constructor

    SimulatorDataManager(const std::string &outputDir, const std::string &travelRootDir);

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

    std::string craneInstructionsSimulationFolder();

    std::string craneInstructionsOutputPath(const std::string &portId, int i);

    std::string cargoDataTempFilePath(const std::string &portId);

    std::string cargoFilePath(const std::string &cargoFileName);

    std::string travelFolder();

    /// Returns a list of travels (i.e. directories) inside travelPath.
    StringVector collectTravels(Errors &errors = Errors::garbageCollector);

    // endregion

    // region Files IO
private:
    void saveErrorsFile(const std::string &fileName, const Errors &errors);
public:
    void saveGeneralErrors(const Errors &errors);  // errors to be saved - not to be collected

    void saveSimulationErrors(const Errors &errors);

    void saveSimulationFinalResults(const StringStringVector &results, const Errors &errors = Errors::garbageCollector);

    /// Creates a crane folder for a single simulation run (must call setTravelInfo before).
    void createTravelCraneFolder();

    /// Create all output folders needed for runSimulations.
    void createOutputFolders(Errors &errors);

    /// Remove unneeded output folders (temp, and errors if not generated).
    void cleanOutputFolders(Errors &errors = Errors::garbageCollector);

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_SIMULATORDATAMANAGER_H
