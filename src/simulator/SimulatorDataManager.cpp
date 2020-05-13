//
// Created by t-yabeny on 5/2/2020.
//

#include "SimulatorDataManager.h"
#include <algorithm>
#include "../common/utils/UtilFunctions.h"
#include "../common/io/FileReader.h"
#include "../common/io/ObjectsReader.h"

// region Constructors

SimulatorDataManager::SimulatorDataManager(const std::string &outputDir, const std::string &travelRootDir) : outputDir(outputDir),
                                                                                                             travelRootDir(travelRootDir) {}

// endregion

// region Getters and setters

void SimulatorDataManager::setTravelName(const std::string &travelName) {
    SimulatorDataManager::travelName = travelName;
    shipPlanFilepath = "";  // reset
    shipRouteFilepath = "";
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
    if (shipPlanFilepath != "") {  // already calculated
        return shipPlanFilepath;
    }

    auto travelFiles = getFilesFromDirectory(travelFolder());

    for (auto& travelFile : travelFiles) {
        if (endsWith(travelFile, ".ship_plan")) {
            shipPlanFilepath = travelFile;
            return travelFile;
        }
    }

    return "";  // not found
}

std::string SimulatorDataManager::shipRoutePath() {
    if (shipRouteFilepath != "") {  // already calculated
        return shipRouteFilepath;
    }

    auto travelFiles = getFilesFromDirectory(travelFolder());

    for (auto& travelFile : travelFiles) {
        if (endsWith(travelFile, ".route")) {
            shipRouteFilepath = travelFile;
            return travelFile;
        }
    }

    return "";
}

std::string SimulatorDataManager::craneInstructionsRootFolder() {  // TODO: maybe we need to remove the /crane_instructions (generate under outputDir directly)
    return outputDir + "/crane_instructions";
}

std::string SimulatorDataManager::craneInstructionsSimulationFolder() {
    return craneInstructionsRootFolder() + "/" + algorithmName + "_" + travelName + "_crane_instructions";
}

std::string SimulatorDataManager::craneInstructionsOutputPath(const std::string &portId, int i) {
    return craneInstructionsSimulationFolder() + "/" + portId + "_" + intToStr(i) + ".crane_instructions";
}

std::string SimulatorDataManager::createCargoDataTempFilePath(const std::string &portId) {
    std::string filePath = tempFolder() + "/" + portId + "_dummy";
    createEmptyFile(filePath);
    return filePath;
}

std::string SimulatorDataManager::cargoFilePath(const std::string &cargoFileName) {
    if (endsWith(cargoFileName, "dummy")) {
        std::string fileName = extractFilenameFromPath(cargoFileName);  // TODO: isn't that already the name only?
        return tempFolder() + "/" + fileName;
    }
    return travelFolder() + "/" + cargoFileName;
}

std::string SimulatorDataManager::travelFolder() {
    return travelRootDir + "/" + travelName;
}

// endregion

// region Functions

bool SimulatorDataManager::isTravelValid(Errors &errors) {
    if (!isDirectoryExists(travelFolder())) {
        errors.addError({ErrorFlag::Travel_InvalidDirectory, travelName});
        return false;
    }

    Errors tempErrors;

    readShipPlanFromFile(shipPlanPath(), tempErrors);
    if (tempErrors.hasFatalError()) {
        errors.addErrors(tempErrors);
        errors.addError({ErrorFlag::Travel_FatalInput, travelName, "Ship Plan"});
        return false;
    }

    readShipRouteFromFile(shipRoutePath(), tempErrors);
    if (tempErrors.hasFatalError()) {
        errors.addErrors(tempErrors);
        errors.addError({ErrorFlag::Travel_FatalInput, travelName, "Ship Route"});
        return false;
    }

    return true;
}

StringVector SimulatorDataManager::collectLegalTravels(Errors &errors) {
    StringVector legalTravels;

    if (travelRootDir == "") {  // no travel path supplied
        errors.addError(ErrorFlag::SimulationInit_InvalidTravelPath);
        return legalTravels;
    }
    StringVector travels = getFilesFromDirectory(travelRootDir);
    if (travels.empty()) {
        errors.addError(ErrorFlag::SimulationInit_InvalidTravelPath);
        return legalTravels;
    }

    for (auto& travel: travels) {
        travelName = extractFilenameFromPath(travel);
        if (isTravelValid(errors)) {
            legalTravels.push_back(travel);
        }
        errors.addTravelLog(travelName);
    }

    if (legalTravels.empty()) {
        errors.addError(ErrorFlag::SimulationInit_AllTravelsAreInvalid);
    }

    return legalTravels;
}

/// Sorts a string vector of .cargo_files by their numbers. Names must be valid, and all belong to each port.
void sortCargoFilesByNumber(StringVector &stringVector) {
    sort(stringVector.begin(), stringVector.end(),
         [](const std::string &a, const std::string &b) -> bool {
             std::string numA = a.substr(6, a.length() - 17);  // dirty trick to extract the number
             std::string numB = b.substr(6, b.length() - 17);
             return strToInt(numA) < strToInt(numB);
         });
}

StringToStringVectorMap SimulatorDataManager::getCargoDataFiles(Errors &errors) {
    StringToStringVectorMap map;

    StringVector files = getFilesFromDirectory(travelFolder());

    for (std::string &file : files) {
        std::string fileName = extractFilenameFromPath(file, false);

        if (!isCargoDataFileFormat(fileName)) {
            if (endsWith(fileName, ".route") || endsWith(fileName, ".ship_plan")) {
                continue;  // we except to see this, so just ignore
            }

            std::cout << "Warning: invalid file in travel folder: " << fileName << std::endl;
            errors.addError({ErrorFlag::Travel_UnknownFile, travelName, fileName});
            continue;
        }

        std::string portId = fileName.substr(0, 5);

        if (map.find(portId) == map.end()) {  // port not encountered yet
            map[portId] = StringVector();
        }

        map[portId].push_back(fileName);  // we push in correct order because files are sorted
    }

    // now we have mapping from AAAAA -> AAAAA_17, AAAAA_2 (unsorted because 17 < 2 but alphabetically 2 < 17)
    for (auto &entry: map) {
        sortCargoFilesByNumber(entry.second);  // sort for each port separately
    }

    return map;
}

// endregion

// region Files

void SimulatorDataManager::saveErrorsFile(const std::string &fileName, const Errors &errorsToWrite) {
    StringVector errorMessages = errorsToWrite.toString();

    std::string filePath = errorsFolder() + "/" + fileName;
    writeFile(filePath, errorMessages);
}

void SimulatorDataManager::saveGeneralErrors(const Errors &errorsToWrite) {
    saveErrorsFile("GeneralErrors", errorsToWrite);
}

void SimulatorDataManager::saveSimulationErrors(const Errors &errorsToWrite) {
    std::string fileName = algorithmName + "_" + travelName + "_errors";
    saveErrorsFile(fileName, errorsToWrite);
}

void SimulatorDataManager::saveSimulationResults(const StringStringVector &results) {
    writeFile(outputDir + "/simulation.results.csv", results);
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

void SimulatorDataManager::cleanOutputFolders() {
    Errors errors;
    bool res = removeFolder(tempFolder());

    bool res2 = true;
    if (isFolderEmpty(errorsFolder())) {
        res2 = removeFolder(errorsFolder());
    }

    if (!(res && res2)) {
        errors.addError(ErrorFlag::SimulationCleanup_OutputDirectoriesCleaningFailed);
    }

    // errors are not sent to anywhere, but keep it for later maybe
}

// endregion