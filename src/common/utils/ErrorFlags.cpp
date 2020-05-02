//
// Created by t-yabeny on 5/1/2020.
//

#include "ErrorFlags.h"


std::string flagToString(ErrorFlag flag);

StringVector errorFlagsToString(int errorFlags) {
    StringVector errors;
    for (int i = 0; i < MAX_ERROR_BIT; i++) {
        int isBitEnabled = errorFlags & (1 << i);

        if (isBitEnabled) {
            std::string errorMsg = flagToString(static_cast<ErrorFlag>(isBitEnabled));
            errors.push_back(errorMsg);
        }
    }

    return errors;
}

StringVector errorsVectorToString(std::vector<ErrorFlag> errorFlagsVector) {
    StringVector errors;
    for (ErrorFlag flag : errorFlagsVector) {
        std::string errorMsg = flagToString(flag);
        errors.push_back(errorMsg);
    }

    return errors;
}

bool hasFlag(int errorsFlag, ErrorFlag flag) {
    return errorsFlag & flag;
}

int errorsVectorToErrorsFlag(std::vector<ErrorFlag> errorFlagsVector) {
    int errors;
    for (ErrorFlag flag : errorFlagsVector) {
        errors |= flag;
    }

    return errors;
}

/// Converts a single flag to a string.
std::string flagToString(ErrorFlag flag) {
    switch (flag) {
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
            return "Containers at port warning: last port has waiting containers, ignored";

        case ContainersAtPort_ContainersExceedsShipCapacity:
            return "Containers at port warning: ship is at full capacity, far containers will not be loaded";

        case SimulationInit_OutputDirectoriesCreationFailed:
            return "Simulation initialization fatal error: couldn't initialize output folders";
        case SimulationInit_InvalidTravelPath:
            return "Simulation initialization fatal error: couldn't find any travel directory";
        case SimulationCleanup_OutputDirectoriesCleaningFailed:
            return "Simulation cleanup error: couldn't remove temporary directories";

        default:
            return "INVALID ERROR";
    }
}

bool containsFatalError(int errorFlags) {  // TODO: check that the & and | return the desired boolean value
    return (errorFlags & ShipPlan_FatalError) | (errorFlags & ShipRoute_FatalError) | (errorFlags & ShipRoute_FatalError_SinglePort);
}

bool containsFatalError(std::vector<ErrorFlag> errorFlagsVector) {
    for (ErrorFlag flag : errorFlagsVector) {
        if (containsFatalError(flag))
            return true;
    }

    return false;
}