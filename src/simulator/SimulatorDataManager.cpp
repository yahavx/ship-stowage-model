//
// Created by t-yabeny on 5/2/2020.
//

#include "SimulatorDataManager.h"
#include "../common/utils/UtilFunctions.h"
#include "../common/io/FileReader.h"

// region Constructors

SimulatorDataManager::SimulatorDataManager(const std::string &outputDir, const std::string &travelRootDir) : outputDir(outputDir),
                                                                                                             travelRootDir(travelRootDir) {}

// endregion

// region Getters and setters

void SimulatorDataManager::setTravelName(const std::string &travelName) {
    SimulatorDataManager::travelName = travelName;
}

void SimulatorDataManager::setAlgorithmName(const std::string &algorithmName) {
    SimulatorDataManager::algorithmName = algorithmName;
}

// endregion

// region Paths generator

std::string SimulatorDataManager::tempFolder() {
    return outputDir + "/" + "temp";
}

std::string SimulatorDataManager::errorsFolder() {
    return outputDir + "/" + "errors";
}

std::string SimulatorDataManager::shipPlanPath() {
    return travelFolder() + "/Plan";
}

std::string SimulatorDataManager::shipRoutePath() {
    return travelFolder() + "/Route";
}

std::string SimulatorDataManager::craneInstructionsRootFolder() {
    return outputDir + "/crane_instructions";
}

std::string SimulatorDataManager::craneInstructionsSimulationFolder() {
    return craneInstructionsRootFolder() + "/" + algorithmName + "_" + travelName + "_crane_instructions";
}

std::string SimulatorDataManager::craneInstructionsOutputPath(const std::string &portId, int i) {
    return craneInstructionsSimulationFolder() + "/" + portId + "_" + intToStr(i) + ".crane_instructions";
}

std::string SimulatorDataManager::cargoDataTempFilePath(const std::string &portId) {
    return tempFolder() + "/" + portId + "_0.cargo_data";
}

std::string SimulatorDataManager::cargoFilePath(const std::string &cargoFileName) {
    return travelFolder() + "/" + cargoFileName;
}

std::string SimulatorDataManager::travelFolder() {
    return travelRootDir + "/" + travelName;
}

StringVector SimulatorDataManager::collectTravels(Errors &errors) {
    if (travelRootDir == "") {  // no travel path supplied
        errors.addError(ErrorFlag::SimulationInit_InvalidTravelPath);
    }
    StringVector files = getFilesFromDirectory(travelRootDir);
    if (files.empty()) {
        errors.addError(ErrorFlag::SimulationInit_InvalidTravelPath);
    }

    return files;
}

// endregion

// region Files
void SimulatorDataManager::saveErrorsFile(const std::string &fileName, const Errors &errors) {
    StringVector errorMessages = errors.toString();

    std::string filePath = errorsFolder() + "/" + fileName;
    writeFile(filePath, errorMessages);
}

void SimulatorDataManager::saveGeneralErrors(const Errors &errors) {
    saveErrorsFile("GeneralErrors", errors);
}

void SimulatorDataManager::saveSimulationErrors(const Errors &errors) {
    std::string fileName = algorithmName + "_" + travelName + "_errors";
    saveErrorsFile(fileName, errors);
}

void SimulatorDataManager::saveSimulationFinalResults(const StringStringVector &results, const Errors &errors) {
    writeFile(outputDir + "/simulation.results.csv", results);
    if (errors.hasErrors()) {
        saveGeneralErrors(errors);
    }
}

void SimulatorDataManager::createTravelCraneFolder() {
    createFolder(craneInstructionsSimulationFolder());
}

void SimulatorDataManager::createOutputFolders(Errors &errors) {

    bool res = createFolder(craneInstructionsRootFolder());
    bool res2 = createFolder(tempFolder());
    bool res3 = createFolder(errorsFolder());
    if (!(res && res2 && res3)) {
        errors.addError(ErrorFlag::SimulationInit_OutputDirectoriesCreationFailed);
    }
}

void SimulatorDataManager::cleanOutputFolders(Errors &errors) {
    bool res = removeFolder(tempFolder());

    bool res2;
    if (isFolderEmpty(errorsFolder())) {
        res2 = removeFolder(errorsFolder());
    }

    if (!(res && res2)) {
        errors.addError(ErrorFlag::SimulationInit_OutputDirectoriesCreationFailed);
    }
}

// endregion