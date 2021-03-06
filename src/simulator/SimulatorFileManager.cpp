//
// Created by t-yabeny on 5/2/2020.
//

#include "SimulatorFileManager.h"
#include <algorithm>
#include "../common/utils/UtilFunctions.h"
#include "../common/io/FileReader.h"
#include "../common/io/ObjectsReader.h"
#include "Travel.h"

// region Constructors

SimulatorFileManager::SimulatorFileManager(const std::string &outputDir, const std::string &travelRootDir) : outputDir(outputDir),
                                                                                                             travelRootDir(travelRootDir) {}

SimulatorFileManager::SimulatorFileManager(const std::string &outputDir, const std::string &travelRootDir, const std::string &travelName,
                                           const std::string &algorithmName) : outputDir(outputDir), travelRootDir(travelRootDir) {
    setTravelName(travelName);
    setAlgorithmName(algorithmName);
}

// endregion

// region Getters and setters

void SimulatorFileManager::setTravelName(const std::string &travelName) {
    SimulatorFileManager::travelName = travelName;
    shipPlanFilepath = "";  // reset
    shipRouteFilepath = "";
}

void SimulatorFileManager::setAlgorithmName(const std::string &algorithmName) {
    SimulatorFileManager::algorithmName = algorithmName;
}

// endregion

// region Paths generator

std::string SimulatorFileManager::tempFolder() {
    return outputDir + "/" + "temp";
}

std::string SimulatorFileManager::errorsFolder() {
    return outputDir + "/" + "errors";
}

std::string SimulatorFileManager::shipPlanPath() {
    if (shipPlanFilepath != "") {  // already calculated
        return shipPlanFilepath;
    }

    auto travelFiles = getFilesFromDirectory(travelFolder());

    for (auto &travelFile : travelFiles) {
        if (endsWith(travelFile, ".ship_plan")) {
            shipPlanFilepath = travelFile;
            return travelFile;
        }
    }

    return "";  // not found
}

std::string SimulatorFileManager::shipRoutePath() {
    if (shipRouteFilepath != "") {  // already calculated
        return shipRouteFilepath;
    }

    auto travelFiles = getFilesFromDirectory(travelFolder());

    for (auto &travelFile : travelFiles) {
        if (endsWith(travelFile, ".route")) {
            shipRouteFilepath = travelFile;
            return travelFile;
        }
    }

    return "";
}

std::string SimulatorFileManager::craneInstructionsRootFolder() {
    return outputDir; // +"/crane_instructions"
}

std::string SimulatorFileManager::craneInstructionsSimulationFolder() {
    return craneInstructionsRootFolder() + "/" + algorithmName + "_" + travelName + "_crane_instructions";
}

std::string SimulatorFileManager::craneInstructionsOutputPath(const std::string &portId, int i) {
    return craneInstructionsSimulationFolder() + "/" + portId + "_" + intToStr(i) + ".crane_instructions";
}

std::string SimulatorFileManager::createCargoDataTempFilePath(const std::string &portId) {
    std::string filePath = tempFolder() + "/" + portId + "_dummy";
    createEmptyFile(filePath);
    return filePath;
}

std::string SimulatorFileManager::cargoFilePath(const std::string &cargoFileName) {
    return travelFolder() + "/" + cargoFileName;
}

std::string SimulatorFileManager::travelFolder() {
    return travelRootDir + "/" + travelName;
}

// endregion

// region Functions

bool SimulatorFileManager::isTravelValid(ShipPlan &shipPlan, ShipRoute &route, Errors &errors) {
    if (!isDirectoryExists(travelFolder())) {
        errors.addError({ErrorFlag::Travel_InvalidDirectory, travelName});
        return false;
    }

    Errors planErrors, routeErrors;

    bool valid = true;

    shipPlan = readShipPlanFromFile(shipPlanPath(), planErrors);
    errors.addErrors(planErrors);
    if (planErrors.hasFatalError()) {
        errors.addError({ErrorFlag::Travel_FatalInput, travelName, "Ship Plan"});
        valid = false;
    }

    route = readShipRouteFromFile(shipRoutePath(), routeErrors);
    errors.addErrors(routeErrors);
    if (routeErrors.hasFatalError()) {
        errors.addError({ErrorFlag::Travel_FatalInput, travelName, "Ship Route"});
        valid = false;
    }

    return valid;
}

std::vector<Travel> SimulatorFileManager::collectLegalTravels(Errors &errors) {
    std::vector<Travel> legalTravels;

    if (travelRootDir == "") {  // no travel path supplied
        errors.addError({ErrorFlag::SimulationInit_InvalidTravelPath, travelRootDir});
        return legalTravels;
    }
    StringVector travels = getFilesFromDirectory(travelRootDir);
    if (travels.empty()) {
        errors.addError({ErrorFlag::SimulationInit_InvalidTravelPath, travelRootDir});
        return legalTravels;
    }

    for (auto &travel: travels) {
        ShipPlan plan;
        ShipRoute route;
        this->setTravelName(extractFilenameFromPath(travel));

        if (this->isTravelValid(plan, route, errors)) {
            legalTravels.emplace_back(travel, plan, route);
        }
        errors.addTravelLog(travelName);  // Adds if needed
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

StringToStringVectorMap SimulatorFileManager::getCargoDataFiles(Errors &errors) {  // TODO: this should only return the list of files
    StringToStringVectorMap map;

    StringVector files = getFilesFromDirectory(travelFolder());

    for (std::string &file : files) {
        std::string fileName = extractFilenameFromPath(file, false);

        if (!isCargoDataFileFormat(fileName)) {
            if (endsWith(fileName, ".route") || endsWith(fileName, ".ship_plan")) {
                continue;  // we except to see this, so just ignore
            }

//            std::cout << "Warning: invalid file in travel folder: " << fileName << std::endl;
            errors.addError({ErrorFlag::Travel_UnknownFile, travelName, fileName});
            continue;
        }

        std::string portId = fileName.substr(0, 5);

        if (map.find(portId) == map.end()) {  // port not encountered yet
            map[portId] = StringVector();
        }

        map[portId].push_back(fileName);
    }

    // now we have mapping from AAAAA -> AAAAA_17, AAAAA_2 (unsorted because 17 < 2 but alphabetically 2 < 17)
    for (auto &entry: map) {
        sortCargoFilesByNumber(entry.second);  // sort for each port separately
    }

    return map;
}

bool SimulatorFileManager::isCargoDataFileFormat(const std::string &fileName) {
    if (!endsWith(fileName, ".cargo_data")) {
        return false;
    }

    if (!isEnglishWord(fileName.substr(0, 5))) {  // no port name at start
        return false;
    }

    if (fileName[5] != '_') {
        return false;
    }

    int size = fileName.length();
    std::string num = fileName.substr(6, size - 17);  // gets the supposed number from the file name

    if (!isInteger(num))
        return false;

    return strToInt(num) != 0;
}

// endregion

// region Files

void SimulatorFileManager::saveErrorsFile(const std::string &fileName, const Errors &errorsToWrite) {
    StringVector errorMessages = errorsToWrite.toString();

    std::string filePath = errorsFolder() + "/" + fileName;
    writeFile(filePath, errorMessages);
}

void SimulatorFileManager::saveGeneralErrors(const Errors &errorsToWrite) {
    saveErrorsFile("GeneralErrors", errorsToWrite);
}

void SimulatorFileManager::saveSimulationErrors(const Errors &errorsToWrite) {
    std::string fileName = algorithmName + "_" + travelName + "_errors";
    saveErrorsFile(fileName, errorsToWrite);
}

void SimulatorFileManager::saveSimulationResults(const StringStringVector &results) {
    writeFile(outputDir + "/simulation.results", results);
}

void SimulatorFileManager::createTravelCraneFolder() {
    createFolder(craneInstructionsSimulationFolder());
}

void SimulatorFileManager::createOutputFolders(Errors &errors) {
//    bool res = createFolder(craneInstructionsRootFolder());
    bool res = createFolder(outputDir);
    bool res2 = createFolder(tempFolder());
    bool res3 = createFolder(errorsFolder());
    if (!(res && res2 && res3)) {
        errors.addError(ErrorFlag::SimulationInit_OutputDirectoriesCreationFailed);
    }
}

void SimulatorFileManager::cleanOutputFolders() {
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