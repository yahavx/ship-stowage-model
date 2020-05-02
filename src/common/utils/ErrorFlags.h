//
// Created by t-yabeny on 5/1/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ERRORFLAGS_H
#define SHIP_STOWAGE_MODEL_ERRORFLAGS_H


#include "Definitions.h"

#define MAX_ERROR_BIT 30

enum ErrorFlag {
    ShipPlan_InvalidFloorHeight = 1 << 0,
    ShipPlan_InvalidXYCoordinates = 1 << 1,
    ShipPlan_BadLineFormat = 1 << 2,
    ShipPlan_FatalError = 1 << 3,

    ShipRoute_TwoConsecutiveSamePort = 1 << 5,
    ShipRoute_BadPortSymbol = 1 << 6,
    ShipRoute_FatalError = 1 << 7,
    ShipRoute_FatalError_SinglePort = 1 << 8,

    ContainersAtPort_DuplicateID = 1 << 10,
    ContainersAtPort_IDAlreadyOnShip = 1 << 11,

    CargoData_MissingOrBadWeight = 1 << 12,
    CargoData_MissingOrBadPortDest = 1 << 13,
    CargoData_MissingContainerID = 1 << 14,
    CargoData_BadContainerID = 1 << 15,
    CargoData_InvalidFile = 1 << 16,
    CargoData_LastPortHasContainers = 1 << 17,

    ContainersAtPort_ContainersExceedsShipCapacity = 1 << 18,

    SimulationInit_OutputDirectoriesCreationFailed = 1 << 20,
    SimulationInit_InvalidTravelPath = 1 << 21,
    SimulationCleanup_OutputDirectoriesCleaningFailed = 1 << 22
};


/// Converts a int that represents a error flags vector, to a string vector, in which each entry represents one error.
StringVector errorFlagsToString(int errorFlags);

/// Converts an error flags vector to a string vector, in which each entry represents one error.
StringVector errorsVectorToString(std::vector<ErrorFlag> errorFlagsVector);

/// Converts an errors vector to a errors flag (int). If an error appears multiple times, it is treated as once.
int errorsVectorToErrorsFlag(std::vector<ErrorFlag> errorFlagsVector);

/// Checks if an errors flag contains a specific flag.
bool hasFlag(int errorsFlag, ErrorFlag flag);

/// Returns true if the error flags contains a fatal error.
bool containsFatalError(int errorFlags);

/// Returns true if the error flags vector contains a fatal error.
bool containsFatalError(std::vector<ErrorFlag> errorFlagsVector);


#endif //SHIP_STOWAGE_MODEL_ERRORFLAGS_H
