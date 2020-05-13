//
// Created by yahav on 13/05/2020.
//

#include "Error.h"
#include "UtilFunctions.h"

// region Error categories

int c_initFatalError = ShipPlan_FatalError_NoFileOrInvalidFirstLine | ShipPlan_FatalError_DuplicateData | ShipRoute_FatalError_NoFileOrNoLegalPorts |
                       ShipRoute_FatalError_SinglePort;

longUInt c_algorithmInstructionErrors =
        AlgorithmError_CraneOperationWithInvalidId | AlgorithmError_InvalidCraneOperation | AlgorithmError_LeftContainersAtPort
        | AlgorithmError_ContainerIdAlreadyOnShip | AlgorithmError_ContainerIdNotExistsOnPort | AlgorithmError_ContainerIdNotExistsOnShip
        | AlgorithmError_RejectedGoodContainer | AlgorithmError_LoadAboveNotLegal | AlgorithmError_UnloadNoContainersAtPosition | AlgorithmError_UnloadBadId
        | AlgorithmError_UnloadBadPosition | AlgorithmError_InvalidXYCoordinates | AlgorithmError_MoveNoContainersAtPosition | AlgorithmError_MoveBadId
        | AlgorithmError_MoveAboveNotLegal | AlgorithmError_TriedToLoadButShouldReject | AlgorithmError_UnloadedAndDidntLoadBack
        | AlgorithmError_WeightBalancerRejectedOperation;  // | AlgorithmError_ExtraReport | AlgorithmError_MissingReport // TODO: decide if we count it as algorithm errors (dangerous)

longUInt c_algorithmFileErrors = ReadOperations_InvalidFile | ReadOperations_InsufficientRowData | ReadOperations_InsufficientRowData_MoveOp
                                 | ReadOperations_InvalidOperationType | ReadOperations_InvalidShipPosition;

longUInt c_algorithmErrors = c_algorithmInstructionErrors | c_algorithmFileErrors;

// endregion

// region Error Prefixes

const std::string shipPlanError = "\t[Ship Plan Error] ";
const std::string shipPlanFatalError = "\t[Ship Plan Fatal Error] ";

const std::string shipRouteError = "\t[Ship Route Error] ";
const std::string shipRouteFatalError = "\t[Ship Route Fatal Error] ";

const std::string containersAtPortError = "\t[Containers At Port Error] ";  // Errors after reading cargo data file (not handled by file reader)
const std::string cargoDataError = "\t[Cargo Data Error] ";  // Errors during cargo data file read

const std::string simulatorError = "\t[Simulator Error] ";
const std::string simulatorFatalError = "\t[Simulator Fatal Error] ";

const std::string travelError = "\t[Travel Error] ";
const std::string travelFatalError = "\t[Travel Fatal Error] ";

const std::string algorithmError = "\t[Algorithm Error] ";
const std::string algorithmOutputError = "\t[Algorithm Output Error] ";

const std::string dynamicLoadError = "\t[Dynamic Load Error] ";
const std::string dynamicLoadFatalError = "\t[Dynamic Load Error] ";

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
        errorMsg = "\t[Algorithm Report] The algorithm reported the following errors: " + intToStr(errorNumbers[0]);

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
            return shipPlanError + "Data row exceeds the maximum available containers in a spot, ignoring (E0)";
        case ShipPlan_InvalidXYCoordinates:
            return shipPlanError + "Data row exceeds the ship dimensions, ignoring (E1)";
        case ShipPlan_BadLineFormat:
            return shipPlanError + "Data row is in invalid format, ignoring (E2)";
        case ShipPlan_FatalError_NoFileOrInvalidFirstLine:
            return shipPlanFatalError + "Invalid first line, or couldn't read file (E3)";
        case ShipPlan_FatalError_DuplicateData:
            return shipPlanFatalError + "Duplicate data rows, about position (" + param1 + ", " + param2 + ") (E4)";

        case ShipRoute_TwoConsecutiveSamePort:
            return shipRouteError + "Port '" + param1 + "' appeared twice in a row, ignoring the latter (E5)";
        case ShipRoute_BadPortSymbol:
            return shipRouteError + "Invalid port symbol format '" + param1 + "' (E6)";
        case ShipRoute_FatalError_NoFileOrNoLegalPorts:
            return shipRouteFatalError + "No ports in the route, or couldn't read file (E7)";
        case ShipRoute_FatalError_SinglePort:
            return shipRouteFatalError + "Only one port appears in the route (8)";

        case ContainersAtPort_DuplicateID:
            return containersAtPortError + "Container with the same ID ('" + param1 + "') was already found on port, rejecting (E10)";
        case ContainersAtPort_IDAlreadyOnShip:
            return containersAtPortError + "Container with the same ID ('" + param1 + "') is already on the ship, rejecting (E11)";
        case ContainersAtPort_MissingOrBadWeight:
            return containersAtPortError + "Missing or bad weight, rejecting (E12)";
        case ContainersAtPort_MissingOrBadPortDest:
            return containersAtPortError + "Missing or bad destination port, ignoring (E13)";
        case CargoData_MissingContainerID:
            return cargoDataError + "Container has no ID, ignoring (E14)";
        case ContainersAtPort_BadContainerID:
            return containersAtPortError + "Container '" + param1 + "' ID is not in ISO 6346 format, rejecting (E15)";
        case CargoData_InvalidFile:
            return cargoDataError + "Couldn't read any container from file '" + param1 + "', cargo will only be loaded (E16)";
        case ContainersAtPort_LastPortHasContainers:
            return containersAtPortError + "Last port has awaiting containers, ignoring (E17)";
        case ContainersAtPort_ContainersExceedsShipCapacity:
            return containersAtPortError + "Ship is at full capacity, container '" + param1 + "' is rejected (E18)";
        case ContainersAtPort_ContainerNotOnRoute:
            return containersAtPortError + "Container '" + param1 + "' destination port is '" + param2 + "', which is not on the ship route, rejecting (E19)";
        case ContainersAtPort_ContainerDestinationIsCurrentPort:
            return containersAtPortError + "Container '" + param1 + "' destination is the current port, rejecting (E20)";

            // Our errors

            // Simulation
        case SimulationInit_OutputDirectoriesCreationFailed:
            return simulatorFatalError + "Couldn't initialize output folders";
        case SimulationInit_InvalidTravelPath:
            return simulatorFatalError + "Travel path not supplied, or couldn't find any travel directories";
        case SimulationInit_AllTravelsAreInvalid:
            return simulatorFatalError + "All travels in the travel directory supplied are invalid";
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
        case AlgorithmError_UnloadBadPosition:
            return algorithmError + "Received unload operation of container with ID '" + param1 +
                   "', at (" + param2 + +", " + param3 + ")" + " but the floor specified is not legal";
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
        case AlgorithmError_UnloadedAndDidntLoadBack:
            return algorithmError + "Unloaded containers to port, with a different destination, and didn't load them back to ship";
        case AlgorithmError_ExtraReport:
            return algorithmError + "Algorithm mistakenly reported error E" + param1;
        case AlgorithmError_MissingReport:
            return algorithmError + "Algorithm didn't report error E" + param1 + ", but it should have been";
        case AlgorithmError_WeightBalancerRejectedOperation:
            return algorithmError + param1 + " operation on container '" + param2 + "' was rejected by the ship's weight balance calculator";

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

        case FileInput_TooManyParameters:
            return param1 + "Too many parameters: expected " + param2 +", but received " + param3 +", ignoring the extra parameters";

        case SharedObject_CantLoadSoFile:
            return dynamicLoadError + "Error while loading SO file: '" + param1 + "'";
        case SharedObject_InvalidDirectory:
            return dynamicLoadFatalError + "Couldn't load any algorithm, the directory is empty or invalid: '" + param1 + "'";
        case SharedObject_AlgorithmDidntSelfRegister:
            return dynamicLoadError + "Algorithm '" + param1 + "' didn't register himself, and is unavailable";
        case SharedObject_LoadedMoreThanOneAlgorithm:
            return dynamicLoadFatalError + "Algorithm '" + param1 + "' registered more than once";
        case SharedObject_NoAlgorithmsLoaded:
            return dynamicLoadFatalError + "No algorithm was loaded successfully";

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