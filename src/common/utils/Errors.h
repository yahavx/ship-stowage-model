//
// Created by t-yabeny on 5/1/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ERRORS_H
#define SHIP_STOWAGE_MODEL_ERRORS_H

#include <ostream>
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

    // our errors
    SimulationInit_OutputDirectoriesCreationFailed = 1 << 20,
    SimulationInit_InvalidTravelPath = 1 << 21,
    SimulationCleanup_OutputDirectoriesCleaningFailed = 1 << 22,
    Travel_InvalidDirectory = 1 << 23,
    Travel_InvalidInput = 1 << 24
};

class Error {
public:
    ErrorFlag errorFlag;
    std::string errorMsg;

    // region Constructors

    Error(ErrorFlag flag);

    Error(const std::string &errorMsg);

    Error(ErrorFlag flag, const std::string &errorMsg);
    // endregion

    // region Functions

    std::string toString();

    bool isFlag(ErrorFlag flag);  // kind of equality operator

    bool isFatalError();
    // endregion
};

class Errors {
    std::vector<Error> errorsList;

public:

    // region Functions

    void addError(const Error &error);

    StringVector toString() const;

    bool hasFatalError();

    /// Returns an int that represents all the errors in the list. If an error appears multiple times, it is treated as once.
    int toErrorFlag();

    bool hasNoErrors() const;

    bool hasErrors() const;
    // endregion

    // region Printer

    friend std::ostream &operator<<(std::ostream &os, const Errors &errors);
    // endregion

    static Errors garbageCollector;  // functions can use this as default parameter to ignore errors
};

/// Converts an errors vector to a errors flag (int). If an error appears multiple times, it is treated as once.
int errorsVectorToErrorsFlag(std::vector<ErrorFlag> errorFlagsVector);  // TODO: move this to algorithm?

/// Converts error to a string.
std::string errorFlagsToString(int errorFlags);  // TODO: move this to simulator?


#endif //SHIP_STOWAGE_MODEL_ERRORS_H
