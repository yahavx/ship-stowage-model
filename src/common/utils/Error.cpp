//
// Created by yahav on 13/05/2020.
//

#include "Error.h"
#include "UtilFunctions.h"

// region Error categories

int c_initFatalError =
        ShipPlan_FatalError_NoFileOrInvalidFirstLine
        | ShipPlan_FatalError_DuplicateData
        | ShipRoute_FatalError_NoFileOrNoLegalPorts
        | ShipRoute_FatalError_SinglePort;

longUInt c_algorithmInitErrors =
        AlgorithmError_FailedToInitialize;

longUInt c_algorithmInstructionErrors =
        AlgorithmError_CraneOperationWithInvalidId
        | AlgorithmError_InvalidCraneOperation
        | AlgorithmError_LeftContainersAtPort
        | AlgorithmError_ContainerIdAlreadyOnShip
        | AlgorithmError_ContainerIdNotExistsOnPort
        | AlgorithmError_ContainerIdNotExistsOnShip
        | AlgorithmError_RejectedGoodContainer
        | AlgorithmError_LoadAboveNotLegal
        | AlgorithmError_UnloadNoContainersAtPosition
        | AlgorithmError_UnloadBadId
        | AlgorithmError_UnloadBadPosition
        | AlgorithmError_InvalidXYCoordinates
        | AlgorithmError_MoveNoContainersAtPosition
        | AlgorithmError_MoveBadId
        | AlgorithmError_MoveAboveNotLegal
        | AlgorithmError_TriedToLoadButShouldReject
        | AlgorithmError_UnloadedAndDidntLoadBack
        | AlgorithmError_ShipNotEmptyAtEndOfRoute
        | AlgorithmError_WeightBalancerRejectedOperation
        | AlgorithmError_FailedToInitialize
        | AlgorithmError_LeftContainersAtShip;
        // | AlgorithmError_ExtraReport
        // | AlgorithmError_MissingReport
        // TODO: decide if we count it as algorithm errors (may be dangerous)

longUInt c_algorithmFileErrors =
        ReadOperations_InvalidFile
        | ReadOperations_InsufficientRowData
        | ReadOperations_InsufficientRowData_MoveOp
        | ReadOperations_InvalidOperationType
        | ReadOperations_InvalidShipPosition;

longUInt c_algorithmErrors = c_algorithmInitErrors | c_algorithmInstructionErrors | c_algorithmFileErrors;

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
const std::string dynamicLoadFatalError = "\t[Dynamic Load Fatal Error] ";

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
    this->errors = errorFlags;
}

// endregion

// region Functions

std::string Error::toString() const {
    if (errorMsg != "")
        return errorMsg;

    switch (errorFlag) {
        case Success:
            return "Success";  // This should not happen

        case ShipPlan_InvalidFloorHeight:
            if (param2 == param3)
                return shipPlanError + "Line " + param1 +": number of floors (" + param2 + ") is equal to the number of floors provided in the first line, this data is redundant, ignoring (E0)";
            else
                return shipPlanError + "Line " + param1 +": number of floors (" + param2 + ") is greater than the number of floors provided in the first line (" + param3 + "), ignoring (E0)";

        case ShipPlan_InvalidXYCoordinates:
            return shipPlanError + "Line " + param1 +": position (" + param2 + ", " + param3 + ") exceeds the ship dimensions, ignoring (E1)";
        case ShipPlan_BadLineFormat:
            if (param2 == "<>")
                return shipPlanError + "Line " + param1 + ": data row is in invalid format (insufficient parameters, not integers, etc), ignoring (E2)";
            else
                return shipPlanError + "Line " + param1 + ": data row contains similar data about position (" + param2 + ", " + param3 + "), ignoring (E2)";

        case ShipPlan_FatalError_NoFileOrInvalidFirstLine:
            if (param1 == "<>")
                return shipPlanFatalError + "File contains no data, or couldn't be read (E3)";
            else
                return shipPlanFatalError + "Line " + param1 + ": invalid ship dimensions (E3)";
        case ShipPlan_FatalError_DuplicateData:
            return shipPlanFatalError + "Line " + param1 + ": duplicate data about position (" + param2 + ", " + param3 + ") (E4)";

        case ShipRoute_TwoConsecutiveSamePort:
            return shipRouteError + "Line " + param1 + ": port '" + param2 + "' appeared twice in a row, ignoring the latter (E5)";
        case ShipRoute_BadPortSymbol:
            return shipRouteError + "Line " + param1 + ": invalid port symbol ('" + param2 + "'), ignoring (E6)";
        case ShipRoute_FatalError_NoFileOrNoLegalPorts:
            return shipRouteFatalError + "No legal ports in the route, or couldn't read file (E7)";
        case ShipRoute_FatalError_SinglePort:
            return shipRouteFatalError + "Port '" + param1 + "' is the only legal port in the route (E8)";

        case ContainersAtPort_DuplicateID:
            return containersAtPortError + "Container with the same ID ('" + param1 + "') was already found on port, expecting to receive a reject (E10)";
        case ContainersAtPort_IDAlreadyOnShip:
            return containersAtPortError + "Container with the same ID ('" + param1 + "') is already on the ship, rejected (E11)";
        case ContainersAtPort_MissingOrBadWeight:
            return containersAtPortError + "Container with ID '" + param1 + "' has missing or bad weight, expecting to receive a reject (E12)";
        case ContainersAtPort_MissingOrBadPortDest:
            return containersAtPortError + "Container with ID '" + param1 + "' has bad destination port: not exists, ot invalid format, expecting to receive a reject (E13)";
        case CargoData_MissingContainerID:
            return cargoDataError + "Line " + param1 +": container has no ID, ignoring (E14)";
        case ContainersAtPort_BadContainerID:
            return containersAtPortError + "Container with ID '" + param1 + "', ID is not in ISO 6346 format, expecting to receive a reject (E15)";
        case CargoData_InvalidFile:
            return cargoDataError + "Couldn't read file '" + param1 + "', cargo will only be loaded (E16)";
        case ContainersAtPort_LastPortHasContainers:
            return containersAtPortError + "Last port in route has awaiting containers, ignoring (E17)";
        case ContainersAtPort_ContainersExceedsShipCapacity:
            return containersAtPortError + "Ship is at full capacity, container with ID '" + param1 + "' was rejected (E18)";
        case ContainersAtPort_ContainerNotOnRoute:
            return containersAtPortError + "Container with ID '" + param1 + "' has bad destination port: '" + param2 + "', which is not on the ship route, expecting to receive a reject (E13)";
        case ContainersAtPort_ContainerDestinationIsCurrentPort:
            return containersAtPortError + "Container with ID '" + param1 + "' has bad destination port: it is the current port, expecting to receive a reject (E13)";

            // Our errors

            // Simulation
        case SimulationInit_OutputDirectoriesCreationFailed:
            return simulatorFatalError + "Couldn't initialize output folders (E21)";
        case SimulationInit_InvalidTravelPath:
            if (param1 == "")
                return simulatorFatalError + "Travel path not supplied (E22)";
            else
                return simulatorFatalError + "Couldn't find any travel directories inside the the travel path supplied: '" + param1 + "' (E22)";
        case SimulationInit_AllTravelsAreInvalid:
            return simulatorFatalError + "All travels in the travel directory supplied are invalid (E23)";
        case SimulationCleanup_OutputDirectoriesCleaningFailed:
            return simulatorError + "Couldn't remove temporary directories (E24)";

            // Travel
        case Travel_InvalidDirectory:
            return travelFatalError + "Travel '" + param1 + "' is not a directory, skipping travel (E25)";
        case Travel_FatalInput:
            return travelFatalError + "Travel '" + param1 + "' has an invalid " + param2 + ", or it doesn't exists, skipping travel (E26)";
        case Travel_UnknownFile:
            return travelError + "Travel contains an invalid file ('" + param2 + "'), ignoring (E27)";
        case Travel_CargoData_PortNotInRoute:
            return cargoDataError + "Port '" + param1 + "' has cargo_data files, but doesn't appear in the Route, ignoring (E28)";
        case Travel_CargoData_RemainingFilesAfterFinish:
            return cargoDataError + "Port '" + param1 + "' has " + param2 + " cargo_data files remaining, ignoring (E29)";

            // Algorithm
        case AlgorithmError_CraneOperationWithInvalidId:
            return algorithmError + "Didn't find container with ID " + param1 + " while at port " + param2 + ", and executing crane operation " + param3 + " (E30)";
        case AlgorithmError_InvalidCraneOperation:
            return algorithmError + "Received the following invalid crane instruction: " + param1 + " (E31)";
        case AlgorithmError_LeftContainersAtPort:
            return algorithmError + "Container with ID " + param1+ " (to port '" + param2 + "'),  was left on the current port ('"+ param3+"'), although ship isn't full (E32)";
        case AlgorithmError_ContainerIdAlreadyOnShip:
            return algorithmError + "Tried to load container with ID '" + param1 + "', which is already on the ship (E33)";
        case AlgorithmError_ContainerIdNotExistsOnPort:
            return algorithmError + "Received a load operation on container ID '" + param1 + "', which doesn't exist on the current port (E34)";
        case AlgorithmError_ContainerIdNotExistsOnShip:
            return algorithmError + "Received a move/unload operation on container with ID '" + param1 + "', which doesn't exist on the ship (E35)";
        case AlgorithmError_RejectedGoodContainer:
            return algorithmError + "Received a reject operation on container with ID '" + param1 +
                   "', but apparently it should have been loaded to the ship (E36)";
        case AlgorithmError_LoadAboveNotLegal:
            return algorithmError + "Received a load operation of container with ID '" + param1 +
                   "', to (" + param2 + +", " + param3 + "), but there is no space on top (E37)";
        case AlgorithmError_UnloadNoContainersAtPosition:
            return algorithmError + "Received an invalid unload operation on container with ID '" + param1+ "': tried to pick him from (" + param2 + +", " + param3 + "), but this position in the ship is empty";
        case AlgorithmError_UnloadBadId:
            return algorithmError + "Received an unload operation of container with ID '" + param1 +
                   "', from (" + param2 + +", " + param3 + ")," + " but there is container with non matching ID on top (E39)";
        case AlgorithmError_UnloadBadPosition:
            return algorithmError + "Received an unload operation of container with ID '" + param1 +
                   "', from (" + param2 + +", " + param3 + "), but the floor specified is incorrect (not the top floor at this position) (E40)";
        case AlgorithmError_InvalidXYCoordinates:
            return algorithmError + "Received an operation on container with ID '" + param1 + "', using an illegal position (out of range): (" + param2 + +", " + param3 + ") (E41)";
        case AlgorithmError_MoveNoContainersAtPosition:
            return algorithmError + "Received an invalid move operation on container with ID '" + param1+ "': tried to pick him from (" + param2 + +", " + param3 + "), but this position in the ship is empty";
        case AlgorithmError_MoveBadId:
            return algorithmError + "The top container at (" + param1 + +", " + param2 + ") has a different ID ('" +param3 + "') then the ID supplied ('" +param4 +"')";
        case AlgorithmError_TriedToLoadButShouldReject:
            return algorithmError + "Received a load operation on container with ID '" + param1 + "' from port '" + param2 + "', but it should have been rejected (E44)";
        case AlgorithmError_MoveAboveNotLegal:
            return algorithmError + "Received a move operation on container with ID '" + param1 +"' to position (" + param2 + +", " + param3 + "), but there is no space left (all floors are taken) (E45)";
        case AlgorithmError_UnloadedAndDidntLoadBack:
            return algorithmError + "Container with ID '" + param1 + "' was left on current port, but its not his destination (E46)";
        case AlgorithmError_LoadInvalidFloor:
            return algorithmError +  "Received a load operation of container with ID '" + param1 +
                                     "', to (" + param2 + +", " + param3 + "), but floor specified (" + param4 +") is incorrect (not the lowest available at this position) (E47)";
        case AlgorithmError_ShipNotEmptyAtEndOfRoute:
            return algorithmError + "Ship was not empty at the end of the travel (E48)";
        case AlgorithmError_WeightBalancerRejectedOperation:
            return algorithmError + param1 + " operation on container with ID '" + param2 + "' was rejected by the ship's weight balance calculator (E49)";
        case AlgorithmError_FailedToInitialize:
            return algorithmError + "Algorithm failed to initialize, although the Ship Plan and Route files are valid (E50)";

            // Read packing operations (produced by algorithm)
        case ReadOperations_InvalidFile:
            return algorithmOutputError + "Operations output file was not created by the algorithm (E51)";
        case ReadOperations_InsufficientRowData:
            return algorithmOutputError + "Line " + param1 + ": Data row should contain at least 2 arguments for a reject operation, 5 for a load/unload, and 8 for a move (E52)";
        case ReadOperations_InsufficientRowData_MoveOp:
            return algorithmOutputError + "Data row contains less than 8 arguments, in a move operation " // strings are concatenated
                                          "(format: M, <container id>, <floor>, <X>, <Y>, <floor>, <X>, <Y>) (E53)";
        case ReadOperations_InvalidOperationType:
            return algorithmOutputError + "Line " + param1 + ": operation is invalid: '" + param2 + "' (should be L/U/M/R) (E54)";
        case ReadOperations_InvalidShipPosition:
            return algorithmOutputError + "Line " + param1 + ": received invalid ship " + param2 + " position: '" + param3 + "' (should be an integer) (E55)";

        case FileInput_TooManyParameters:
            return param1 + "Line " + param2 +": too many parameters - expected " + param3 +", but received " + param4 +", ignoring the extra parameters (E56)";

        case SharedObject_CantLoadSoFile:
            return dynamicLoadError + "Error while loading SO file: '" + param1 + "', can't load algorithm (E57)";
        case SharedObject_InvalidDirectory:
            return dynamicLoadFatalError + "Couldn't load any algorithm, the directory is empty or invalid: '" + param1 + "' (E58)";
        case SharedObject_AlgorithmDidntSelfRegister:
            return dynamicLoadError + "Algorithm '" + param1 + "' didn't register himself, and is unavailable (E59)";
        case SharedObject_LoadedMoreThanOneAlgorithm:
            return dynamicLoadError + "Algorithm '" + param1 + "' registered more than once, and therefore is unavailable (E60)";
        case SharedObject_NoAlgorithmsLoaded:
            return dynamicLoadFatalError + "No algorithm was loaded successfully (E61)";

        case AlgorithmError_LeftContainersAtShip:
            return algorithmError + "Container with ID '" + param1 + "' is for current port ('" + param2 + "'), but wasn't unloaded from the ship";

        default:
            return "ERROR NOT SUPPORTED YET";
    }

    return "TO STRING ERROR";
}

bool Error::isCertainFlag(ErrorFlag other) {
    return errorFlag & other;
}

bool Error::isFatalError() {
    return (errorFlag & c_initFatalError) | (errors & c_initFatalError);
}

bool Error::isAlgorithmError() {
    return errorFlag & c_algorithmErrors;
}

bool Error::isSuccess() {
    return errorFlag == ErrorFlag::Success;
}

// endregion
