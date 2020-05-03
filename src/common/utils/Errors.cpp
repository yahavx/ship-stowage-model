//
// Created by t-yabeny on 5/1/2020.
//

#include "Errors.h"
#include "UtilFunctions.h"


// region Error class

Errors Errors::garbageCollector;

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
            return "Ship plan warning: data row exceeds the maximum available containers, ignored";
        case ShipPlan_InvalidXYCoordinates:
            return "Ship plan warning: data row exceeds the ship dimensions, ignored";
        case ShipPlan_BadLineFormat:
            return "Ship plan warning: data row is in invalid format, ignored";
        case ShipPlan_FatalError:
            return "Ship plan fatal error: invalid first line, or couldn't read file, terminating the travel";

        case ShipRoute_TwoConsecutiveSamePort:
            return "Ship route warning: a route appeared twice in a row";
        case ShipRoute_BadPortSymbol:
            return "Ship route warning: invalid port symbol format";
        case ShipRoute_FatalError:
            return "Ship route fatal error: no ports in the route, or couldn't read file, terminating the travel";
        case ShipRoute_FatalError_SinglePort:
            return "Ship route fatal error: only one port appears in the route, terminating the travel";

        case ContainersAtPort_DuplicateID:
            return "Containers at port warning: duplicate container ID found, rejecting";
        case ContainersAtPort_IDAlreadyOnShip:
            return "Containers at port warning: container with the same ID already on the ship, rejecting";

        case CargoData_MissingOrBadWeight:
            return "Cargo data warning: missing or bad weight, ignoring";
        case CargoData_MissingOrBadPortDest:
            return "Cargo data warning: missing or bad destination port, ignoring";
        case CargoData_MissingContainerID:
            return "Cargo data warning: missing container ID, ignoring";
        case CargoData_BadContainerID:
            return "Cargo data warning: container ID is not in ISO 6346 format, ignoring";
        case CargoData_InvalidFile:
            return "Cargo data warning: couldn't read any container from file, cargo will only be loaded";
        case CargoData_LastPortHasContainers:
            return "Containers at port warning: last port has waiting containers, ignoring";

        case ContainersAtPort_ContainersExceedsShipCapacity:
            return "Containers at port warning: ship is at full capacity, far containers will not be loaded";

            // Our errors

        case SimulationInit_OutputDirectoriesCreationFailed:
            return "[Simulator Fatal Error] Couldn't initialize output folders";
        case SimulationInit_InvalidTravelPath:
            return "[Simulator Fatal Error] Travel path not supplied, or couldn't find any travel directories";
        case SimulationCleanup_OutputDirectoriesCleaningFailed:
            return "[Simulator Warning] Couldn't remove temporary directories";

        case Travel_InvalidDirectory:
            return "[Travel Error] Travel '" + param1 + "' is not a directory, skipping";
        case Travel_InvalidInput:
            return "[Travel Error] Travel '" + param1 + "' has an invalid " + param2 +", or it doesn't exists, skipping";

        case AlgorithmError_CraneOperationWithInvalidId:
            return "[Algorithm Error] Didn't find container " + param1 + " while at port " + param2 + ", and executing crane operation " + param3;
        case AlgorithmError_InvalidCraneOperation:
            return "[Algorithm Error] Received illegal crane operation: " + param1;
        case AlgorithmError_LeftContainersAtPort:
            return "[Algorithm Error] Algorithm didn't load all required containers from port " + param1 + ", although ship isn't full";

        default:
            return "ERROR NOT SUPPORTED YET";
    }
}

bool Error::isFlag(ErrorFlag flag) {
    return errorFlag & flag;
}

bool Error::isFatalError() {
    return (errorFlag & ShipPlan_FatalError) | (errorFlag & ShipRoute_FatalError) | (errorFlag & ShipRoute_FatalError_SinglePort);
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

bool Errors::hasFatalError() {
    for (Error error : errorsList) {
        if (error.isFatalError())
            return true;
    }

    return false;
}

bool Errors::hasAlgorithmErrors() {
    int algorithmErrors = AlgorithmError_CraneOperationWithInvalidId | AlgorithmError_InvalidCraneOperation | AlgorithmError_LeftContainersAtPort;

    for (Error &error : errorsList) {
        if (error.errorFlag & algorithmErrors) {
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
