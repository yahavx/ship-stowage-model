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

// region Error categories

int c_initFatalError = ShipPlan_FatalError_NoFileOrInvalidFirstLine | ShipPlan_FatalError_DuplicateData | ShipRoute_FatalError_NoFileOrNoLegalPorts |
                       ShipRoute_FatalError_SinglePort;

longUInt c_algorithmInstructionErrors =
        AlgorithmError_CraneOperationWithInvalidId | AlgorithmError_InvalidCraneOperation | AlgorithmError_LeftContainersAtPort
        | AlgorithmError_ContainerIdAlreadyOnShip | AlgorithmError_ContainerIdNotExistsOnPort | AlgorithmError_ContainerIdNotExistsOnShip
        | AlgorithmError_RejectedGoodContainer | AlgorithmError_LoadAboveNotLegal | AlgorithmError_UnloadNoContainersAtPosition | AlgorithmError_UnloadBadId
        | AlgorithmError_InvalidXYCoordinates | AlgorithmError_MoveNoContainersAtPosition | AlgorithmError_MoveBadId | AlgorithmError_MoveAboveNotLegal
        | AlgorithmError_TriedToLoadButShouldReject;

longUInt c_algorithmFileErrors = ReadOperations_InvalidFile | ReadOperations_InsufficientRowData | ReadOperations_InsufficientRowData_MoveOp
                                 | ReadOperations_InvalidOperationType | ReadOperations_InvalidShipPosition;

longUInt c_algorithmErrors = c_algorithmInstructionErrors | c_algorithmFileErrors;

// endregion

// region Error Prefixes

const std::string shipPlanError = "[Ship Plan Error] ";
const std::string shipPlanFatalError = "[Ship Plan Error] ";

const std::string shipRouteError = "[Ship Route Error] ";
const std::string shipRouteFatalError = "[Ship Route Fatal Error] ";

const std::string containersAtPortError = "[Containers At Port Error] ";

const std::string cargoDataError = "[Cargo Data Error] ";
const std::string cargoDataWarning = "[Cargo Data Warning] ";

const std::string simulatorError = "[Simulator Error] ";
const std::string simulatorFatalError = "[Simulator Fatal Error] ";

const std::string travelError = "[Travel Error] ";
const std::string travelFatalError = "[Travel Fatal Error] ";

const std::string algorithmError = "[Algorithm Error] ";
const std::string algorithmOutputError = "[Algorithm Output Error] ";

// endregion

// region Constructors

Error::Error(ErrorFlag flag) : errorFlag(flag) {}

Error::Error(ErrorFlag errorFlag, const std::string &param1) : errorFlag(errorFlag), param1(param1) {}

Error::Error(ErrorFlag errorFlag, const std::string &param1, const std::string &param2) : errorFlag(errorFlag), param1(param1), param2(param2) {}

Error::Error(ErrorFlag errorFlag, const std::string &param1, const std::string &param2, const std::string &param3) : errorFlag(errorFlag), param1(param1),
                                                                                                                     param2(param2), param3(param3) {}

Error::Error(ErrorFlag errorFlag, const std::string &param1, const std::string &param2, const std::string &param3, const std::string &param4) : errorFlag(
        errorFlag), param1(param1), param2(param2), param3(param3), param4(param4) {}

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
            return shipPlanError + "Data row exceeds the maximum available containers, ignored";
        case ShipPlan_InvalidXYCoordinates:
            return shipPlanError + "Data row exceeds the ship dimensions, ignored";
        case ShipPlan_BadLineFormat:
            return shipPlanError + "Data row is in invalid format, ignoring";
        case ShipPlan_FatalError_NoFileOrInvalidFirstLine:
            return shipPlanFatalError + "Invalid first line, or couldn't read file, terminating the travel";
        case ShipPlan_FatalError_DuplicateData:
            return shipPlanFatalError + "Duplicate data rows, about position (" + param1 + ", " + param2 + ")";

        case ShipRoute_TwoConsecutiveSamePort:
            return shipRouteError + "Port '" + param1 + "' appeared twice in a row, ignoring the latter";
        case ShipRoute_BadPortSymbol:
            return shipRouteError + "Invalid port symbol format '" + param1 + "'";
        case ShipRoute_FatalError_NoFileOrNoLegalPorts:
            return shipRouteFatalError + "No ports in the route, or couldn't read file, terminating the travel";
        case ShipRoute_FatalError_SinglePort:
            return shipRouteFatalError + "Only one port appears in the route, terminating the travel";

        case ContainersAtPort_DuplicateID:
            return containersAtPortError + "Container with the same ID ('" + param1 + "') was already found on port, rejecting";
        case ContainersAtPort_IDAlreadyOnShip:
            return containersAtPortError + "Container with the same ID ('" + param1 + "') is already on the ship, rejecting";
        case ContainersAtPort_MissingOrBadWeight:
            return containersAtPortError + "Missing or bad weight, rejecting";
        case ContainersAtPort_MissingOrBadPortDest:
            return containersAtPortError + "Missing or bad destination port, ignoring";
        case CargoData_MissingContainerID:
            return cargoDataError + "Container has no ID, ignoring";
        case ContainersAtPort_BadContainerID:
            return containersAtPortError + "Container '" + param1 + "' ID is not in ISO 6346 format, rejecting";
        case CargoData_InvalidFile:
            return cargoDataWarning + "Couldn't read any container from file '" + param1 + "', cargo will only be loaded";
        case ContainersAtPort_LastPortHasContainers:
            return containersAtPortError + "Last port has awaiting containers, ignoring";
        case ContainersAtPort_ContainersExceedsShipCapacity:
            return containersAtPortError + "Ship is at full capacity, container '" + param1 + "' is rejected";
        case ContainersAtPort_ContainerNotOnRoute:
            return containersAtPortError + "Container '" + param1 + "' destination port is '" + param2 + "', which is not on the ship route, rejecting";
        case ContainersAtPort_ContainerDestinationIsCurrentPort:
            return containersAtPortError + "Container '" + param1 + "' destination is the current port, rejecting";

            // Our errors

            // Simulation
        case SimulationInit_OutputDirectoriesCreationFailed:
            return simulatorFatalError + "Couldn't initialize output folders";
        case SimulationInit_InvalidTravelPath:
            return simulatorFatalError + "Travel path not supplied, or couldn't find any travel directories";
        case SimulationCleanup_OutputDirectoriesCleaningFailed:
            return simulatorError + "Couldn't remove temporary directories";

            // Travel
        case Travel_InvalidDirectory:
            return travelFatalError + "Travel '" + param1 + "' is not a directory, skipping travel";
        case Travel_FatalInput:
            return travelFatalError + "Travel '" + param1 + "' has an invalid " + param2 + ", or it doesn't exists, skipping travel";
        case Travel_UnknownFile:
            return travelError + "Travel '" + param1 + "' has an invalid file ('" + param2 + "'), ignoring";
        case Travel_CargoData_PortNotInRoute:
            return travelError + "Port '" + param1 + "' has cargo_data files, but doesn't appear in the Route, ignoring";
        case Travel_CargoData_RemainingFilesAfterFinish:
            return travelError + "Port '" + param1 + "' has " + param2 + " cargo_data files remaining, after travel is finished, ignoring";

            // Algorithm
        case AlgorithmError_CraneOperationWithInvalidId:
            return algorithmError + "Didn't find container " + param1 + " while at port " + param2 + ", and executing crane operation " + param3;
        case AlgorithmError_InvalidCraneOperation:
            return algorithmError + "Received the following invalid crane instruction: " + param1;
        case AlgorithmError_LeftContainersAtPort:
            return algorithmError + "At least one container to port '" + param1 + "' was left on port '" + param2 + "', although ship isn't full";
        case AlgorithmError_ContainerIdAlreadyOnShip:
            return algorithmError + "Tried to load container with ID '" + param1 + "', which is already on the ship";
        case AlgorithmError_ContainerIdNotExistsOnPort:
            return algorithmError + "Received a load operation on container ID '" + param1 + "', which doesn't exist on the current port";
        case AlgorithmError_ContainerIdNotExistsOnShip:
            return algorithmError + "Received a move/unload operation on container with ID '" + param1 + "', which doesn't exist on the ship";
        case AlgorithmError_RejectedGoodContainer:
            return algorithmError + "Received a reject operation on container with ID '" + param1 +
                   "', but apparently it should have been loaded to the ship";
        case AlgorithmError_LoadAboveNotLegal:
            return algorithmError + "Received loading operation of container with ID '" + param1 +
                   "', at (" + param2 + +", " + param3 + ")" + " but there is no space on top";
        case AlgorithmError_UnloadBadId:
            return algorithmError + "Received unload operation of container with ID '" + param1 +
                   "', at (" + param2 + +", " + param3 + ")" + " but there is container with non matching ID on top";
        case AlgorithmError_UnloadNoContainersAtPosition:
            return algorithmError + "Received unload operation of container with ID '" + param1 +
                   "', at (" + param2 + +", " + param3 + ")" + " but there are no containers";
        case AlgorithmError_InvalidXYCoordinates:
            return algorithmError + "Received operation on container with ID '" + param2 + "', using an illegal position: (" + param3 + +", " + param4 + ")";
        case AlgorithmError_MoveNoContainersAtPosition:
            break;
        case AlgorithmError_MoveBadId:
            break;
        case AlgorithmError_TriedToLoadButShouldReject:
            return algorithmError + "Try to load container with ID '" + param1 + "' from port '" + param2 + "', but it should have been rejected";


            // Read packing operations (produced by algorithm)
        case ReadOperations_InvalidFile:
            return algorithmOutputError + "Operations output file was not created by the algorithm";
        case ReadOperations_InsufficientRowData:
            return algorithmOutputError + "Data row contains less than 5 arguments (format: <L/U/M/R> <container id>, <floor>, <X>, <Y>";
        case ReadOperations_InsufficientRowData_MoveOp:
            return algorithmOutputError + "Data row contains less than 8 arguments, in a move operation " // strings are concatenated
                                          "(format: M, <container id>, <floor>, <X>, <Y>, <floor>, <X>, <Y>)";
        case ReadOperations_InvalidOperationType:
            return algorithmOutputError + "Operation is invalid: '" + param1 + "' (should be L/U/M/R)";
        case ReadOperations_InvalidShipPosition:
            return algorithmOutputError + "Received invalid ship " + param1 + " position: '" + param2 + "' (should be an integer)";


        default:
            return "ERROR NOT SUPPORTED YET";
    }

    return "TO STRING ERROR";
}

bool Error::isFlag(ErrorFlag flag) {
    return errorFlag & flag;
}

bool Error::isFatalError() {
    return errorFlag & c_initFatalError;
}

bool Error::isAlgorithmError() {
    return errorFlag & c_algorithmErrors;
}

bool Error::isSuccess() {
    return errorFlag == ErrorFlag::Success;
}

// endregion

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

int Errors::toErrorFlag() {
    int errors;
    for (Error error : errorsList) {
        errors |= error.errorFlag;
    }

    return errors;
}

void Errors::setCheckpoint() {
    checkpoint = errorsList.size();
}

void Errors::addSimulationLog(int portVisitNum, const std::string &portId, int totalStops) {
    if (errorsList.size() > checkpoint) {
        std::string logMessage =
                "\nThe following errors were detected on visit number " + intToStr(portVisitNum) + " in port '" + portId + "', which is stop number " +
                intToStr(totalStops) + " since the beginning of the journey:";
        errorsList.insert(errorsList.begin() + checkpoint, logMessage);
    }
}

// endregion

// region Check Error Type

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