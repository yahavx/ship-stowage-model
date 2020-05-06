//
// Created by t-yabeny on 5/1/2020.
//

#include <iostream>
#include "Errors.h"
#include "UtilFunctions.h"


// region Error class

// region Garbage Collector

Errors e_garbageCollector;

// endregion

// region Constructors

Error::Error(ErrorFlag flag) : errorFlag(flag) {}

Error::Error(ErrorFlag errorFlag, const std::string &param1) : errorFlag(errorFlag), param1(param1) {}

Error::Error(ErrorFlag errorFlag, const std::string &param1, const std::string &param2) : errorFlag(errorFlag), param1(param1), param2(param2) {}

Error::Error(ErrorFlag errorFlag, const std::string &param1, const std::string &param2, const std::string &param3) : errorFlag(errorFlag), param1(param1),
                                                                                                                     param2(param2), param3(param3) {}

Error::Error(const std::string &errorMsg) : errorMsg(errorMsg) {}

Error::Error(int errorFlags) {
    IntVector errorNumbers;
    for (int i = 0; i <= MAX_ERROR_BIT; i++) {
        int isBitEnabled = errorFlags & (1 << i);

        if (isBitEnabled) {
            errorNumbers.push_back(i);
        }
    }

    if (!errorNumbers.empty()) {
        errorMsg = "The algorithm reported the following errors: " + intToStr(errorNumbers[0]);

        for (longUInt i = 1; i < errorNumbers.size(); i++) {
            errorMsg += ", " + intToStr(errorNumbers[i]);
        }
    }
}

// endregion

// region Functions

std::string Error::toString() {
    if (errorMsg != "")
        return errorMsg;

    switch (errorFlag) {
        case Success:
            return "Success (this shouldn't appear)";

        case ShipPlan_InvalidFloorHeight:
            return "[Ship Plan Error] Data row exceeds the maximum available containers, ignored";
        case ShipPlan_InvalidXYCoordinates:
            return "[Ship Plan Error] Data row exceeds the ship dimensions, ignored";
        case ShipPlan_BadLineFormat:
            return "[Ship Plan Error] Data row is in invalid format, ignoring";
        case ShipPlan_FatalError_NoFileOrInvalidFirstLine:
            return "[Ship Plan Fatal Error] Invalid first line, or couldn't read file, terminating the travel";
        case ShipPlan_FatalError_DuplicateData:
            return "[Ship Plan Fatal Error] Duplicate data rows, about position (" + param1 + ", " + param2 + ")";

        case ShipRoute_TwoConsecutiveSamePort:
            return "[Ship Route Error] Port '" + param1 + "' appeared twice in a row, ignoring the latter";
        case ShipRoute_BadPortSymbol:
            return "[Ship Route Error] Invalid port symbol format '" + param1 + "'";
        case ShipRoute_FatalError_NoFileOrNoLegalPorts:
            return "[Ship Route Fatal Error] no ports in the route, or couldn't read file, terminating the travel";
        case ShipRoute_FatalError_SinglePort:
            return "[Ship Route Fatal Error] only one port appears in the route, terminating the travel";

        case ContainersAtPort_DuplicateID:
            return "[Containers At Port Error] Container with the same ID ('" + param1 + "') was already found on port, rejecting";
        case ContainersAtPort_IDAlreadyOnShip:
            return "[Containers At Port Error] Container with the same ID ('" + param1 + "') is already on the ship, rejecting";
        case ContainersAtPort_MissingOrBadWeight:
            return "[Containers At Port Error] Missing or bad weight, rejecting";
        case ContainersAtPort_MissingOrBadPortDest:
            return "[Containers At Port Error] Missing or bad destination port, ignoring";
        case CargoData_MissingContainerID:
            return "[Cargo data Error] Container has no ID, ignoring";
        case ContainersAtPort_BadContainerID:
            return "[Containers At Port Error] container '" + param1 + "' ID is not in ISO 6346 format, rejecting";
        case CargoData_InvalidFile:
            return "[Cargo Data Warning] Couldn't read any container from file '" + param1 + "', cargo will only be loaded";
        case ContainersAtPort_LastPortHasContainers:
            return "[Containers At Port Error] Last port has awaiting containers, ignoring";
        case ContainersAtPort_ContainersExceedsShipCapacity:
            return "[Containers At Port Error] Ship is at full capacity, container '" + param1 + "' is rejected";
        case ContainersAtPort_ContainerNotOnRoute:
            return "[Containers At Port Error] Container '" + param1 + "' destination port is '" + param2 + "', which is not on the ship route, rejecting";
        case ContainersAtPort_ContainerDestinationIsCurrentPort:
            return "[Containers At Port Error] Container '" + param1 + "' destination is the current port, rejecting";

            // Our errors

            // Simulation
        case SimulationInit_OutputDirectoriesCreationFailed:
            return "[Simulator Fatal Error] Couldn't initialize output folders";
        case SimulationInit_InvalidTravelPath:
            return "[Simulator Fatal Error] Travel path not supplied, or couldn't find any travel directories";
        case SimulationCleanup_OutputDirectoriesCleaningFailed:
            return "[Simulator Error] Couldn't remove temporary directories";

            // Travel
        case Travel_InvalidDirectory:
            return "[Travel Fatal Error] Travel '" + param1 + "' is not a directory, skipping travel";
        case Travel_FatalInput:
            return "[Travel Fatal Error] Travel '" + param1 + "' has an invalid " + param2 + ", or it doesn't exists, skipping travel";
        case Travel_UnknownFile:
            return "[Travel Error] Travel '" + param1 + "' has an invalid file ('" + param2 + "'), ignoring";
        case Travel_CargoData_PortNotInRoute:
            return "[Travel Error] Port '" + param1 + "' has cargo_data files, but doesn't appear in the Route, ignoring";
        case Travel_CargoData_RemainingFilesAfterFinish:
            return "[Travel Error] Port '" + param1 + "' has " + param2 + " cargo_data files remaining, after travel is finished, ignoring";

            // Algorithm
        case AlgorithmError_CraneOperationWithInvalidId:
            return "[Algorithm Error] Didn't find container " + param1 + " while at port " + param2 + ", and executing crane operation " + param3;
        case AlgorithmError_InvalidCraneOperation:
            return "[Algorithm Error] Received the following invalid crane instruction: " + param1;
        case AlgorithmError_LeftContainersAtPort:
            return "[Algorithm Error] Container to port '" + param1 + "' was left on port '" + param2 + "', although ship isn't full";
        case AlgorithmError_ContainerIdAlreadyOnShip:
            return "[Algorithm Error] Tried to load container with ID '" + param1 + "', which is already on the ship";
        case AlgorithmError_ContainerIdNotExistsOnPort:
            return "[Algorithm Error] Received a load operation on container ID '" + param1 + "', which doesn't exist on the current port";
        case AlgorithmError_ContainerIdNotExistsOnShip:
            return "[Algorithm Error] Received a move/unload operation on container with ID '" + param1 + "', which doesn't exist on the ship";
        case AlgorithmError_RejectedGoodContainer:
            return "[Algorithm Error] Received a reject operation on container with ID '" + param1 +
                   "', but no apparently it should have been loaded to the ship";
        case AlgorithmError_LoadAboveNotLegal:
            return "[Algorithm Error] Received loading operation of container with ID '" + param1 +
                          "', at (" + param2 +  +", " + param3 + ")"  + " but there is no space on top";
        case AlgorithmError_UnloadBadId:
            return "[Algorithm Error] Received unloading operation of container with ID '" + param1 +
                   "', at (" + param2 +  +", " + param3 + ")"  + " but there is container with non matching ID on top";
        case AlgorithmError_UnloadNoContainersAtPosition:
            return "[Algorithm Error] Received unloading operation of container with ID '" + param1 +
                   "', at (" + param2 +  +", " + param3 + ")"  + " but there are no containers";
        case AlgorithmError_InvalidXYCoordinates:
            return "[Algorithm Error] Received operation of container with ID '" + param1 +
                   "', to illegal position: (" + param2 +  +", " + param3 + ")";


            // Read packing operations (produced by algorithm)
        case ReadOperations_InvalidFile:
            return "[Algorithm Output Error] Operations output file was not created by the algorithm";
        case ReadOperations_InsufficientRowData:
            return "[Algorithm Output Error] Data row contains less than 5 arguments (format: <L/U/M/R> <container id>, <floor>, <X>, <Y>";
        case ReadOperations_InsufficientRowData_MoveOp:
            return "[Algorithm Output Error] Data row contains less than 8 arguments, in a move operation " // strings are concatenated
                   "(format: M, <container id>, <floor>, <X>, <Y>, <floor>, <X>, <Y>)";
        case ReadOperations_InvalidOperationType:
            return "[Algorithm Output Error] Operation is invalid: '" + param1 + "' (should be L/U/M/R)";
        case ReadOperations_InvalidShipPosition:
            return "[Algorithm Output Error] Received invalid ship " + param1 + " position: '" + param2 + "' (should be an integer)";


        default:
            return "ERROR NOT SUPPORTED YET";
    }

    return "TO STRING ERROR";
}

bool Error::isFlag(ErrorFlag flag) {
    return errorFlag & flag;
}

bool Error::isFatalError() {
    int fatalError = ShipPlan_FatalError_NoFileOrInvalidFirstLine | ShipPlan_FatalError_DuplicateData | ShipRoute_FatalError_NoFileOrNoLegalPorts |
                     ShipRoute_FatalError_SinglePort;
    return errorFlag & fatalError;
}

bool Error::isAlgorithmError() {
    longUInt algorithmInstructionErrors =
            AlgorithmError_CraneOperationWithInvalidId | AlgorithmError_InvalidCraneOperation | AlgorithmError_LeftContainersAtPort
            | AlgorithmError_ContainerIdAlreadyOnShip | AlgorithmError_ContainerIdNotExistsOnPort | AlgorithmError_ContainerIdNotExistsOnShip
            | AlgorithmError_RejectedGoodContainer;
    longUInt algorithmFileErrors = ReadOperations_InvalidFile | ReadOperations_InsufficientRowData | ReadOperations_InsufficientRowData_MoveOp
                                   | ReadOperations_InvalidOperationType | ReadOperations_InvalidShipPosition;
    longUInt algorithmErrors = algorithmInstructionErrors | algorithmFileErrors;

    return errorFlag & algorithmErrors;
}

bool Error::isSuccess() {
    return errorFlag == ErrorFlag::Success;
}

// endregion

// region Errors class

// region Functions

void Errors::addError(const Error &error) {
    if (error.errorFlag != ErrorFlag::Success || error.errorMsg != "") {  // actual error
        errorsList.push_back(error);
    }
}

StringVector Errors::toString() const {
    StringVector errors;
    for (Error error : errorsList) {
        errors.push_back(error.toString());
    }

    return errors;
}

bool Errors::hasFatalError() {
    for (Error error : errorsList) {
        if (error.isFatalError())
            return true;
    }

    return false;
}

bool Errors::hasAlgorithmErrors() {


    for (Error &error : errorsList) {
        if (error.isAlgorithmError()) {
            return true;
        }
    }

    return false;
}

int Errors::toErrorFlag() {
    int errors;
    for (Error error : errorsList) {
        errors |= error.errorFlag;
    }

    return errors;
}

bool Errors::hasNoErrors() const {
    return errorsList.empty();
}

bool Errors::hasErrors() const {
    return !errorsList.empty();
}
// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const Errors &errors) {
    StringVector errorsStr = errors.toString();
    std::cout << "Errors {" << std::endl;
    for (longUInt i = 0; i < errorsStr.size(); i++) {
        std::cout << "\t" << errorsStr[i] << std::endl;
    }
    std::cout << "}" << std::endl;
    return os;
}
// endregion
// endregion