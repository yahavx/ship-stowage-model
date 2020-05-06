//
// Created by t-yabeny on 5/1/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ERRORS_H
#define SHIP_STOWAGE_MODEL_ERRORS_H

#include <ostream>
#include "Definitions.h"

#define MAX_ERROR_BIT 18  // max that is used by the algorithm (rest are for us)


enum ErrorFlag : longUInt {
    Success = 0,

    ShipPlan_InvalidFloorHeight = 1 << 0,
    ShipPlan_InvalidXYCoordinates = 1 << 1,
    ShipPlan_BadLineFormat = 1 << 2,
    ShipPlan_FatalError_NoFileOrInvalidFirstLine = 1 << 3,
    ShipPlan_FatalError_DuplicateData = 1 << 4,

    ShipRoute_TwoConsecutiveSamePort = 1 << 5,
    ShipRoute_BadPortSymbol = 1 << 6,
    ShipRoute_FatalError_NoFileOrNoLegalPorts = 1 << 7,
    ShipRoute_FatalError_SinglePort = 1 << 8,

    ContainersAtPort_DuplicateID = 1 << 10,
    ContainersAtPort_IDAlreadyOnShip = 1 << 11,

    ContainersAtPort_MissingOrBadWeight = 1 << 12,
    ContainersAtPort_MissingOrBadPortDest = 1 << 13,
    CargoData_MissingContainerID = 1 << 14,
    ContainersAtPort_BadContainerID = 1 << 15,
    CargoData_InvalidFile = 1 << 16,
    ContainersAtPort_LastPortHasContainers = 1 << 17,
    ContainersAtPort_ContainersExceedsShipCapacity = 1 << 18,
    ContainersAtPort_ContainerNotOnRoute = 1ULL << 49,  // TODO: check what to do with this
    ContainersAtPort_ContainerDestinationIsCurrentPort = 1ULL << 50,

    // our errors TODO: make sure the algorithm can't receive any by mistake
    SimulationInit_OutputDirectoriesCreationFailed = 1 << 19,
    SimulationInit_InvalidTravelPath = 1 << 20,
    SimulationCleanup_OutputDirectoriesCleaningFailed = 1 << 21,

    Travel_InvalidDirectory = 1 << 22,
    Travel_FatalInput = 1 << 23,
    Travel_UnknownFile = 1 << 24,
    Travel_CargoData_PortNotInRoute = 1 << 25,
    Travel_CargoData_RemainingFilesAfterFinish = 1 << 26,

    AlgorithmError_CraneOperationWithInvalidId = 1 << 28,  // TODO: this should be mapped to error 32/33
    AlgorithmError_InvalidCraneOperation = 1 << 29,
    AlgorithmError_LeftContainersAtPort = 1 << 30,
    AlgorithmError_ContainerIdAlreadyOnShip = 1ULL << 31,
    AlgorithmError_ContainerIdNotExistsOnPort = 1ULL << 32,
    AlgorithmError_ContainerIdNotExistsOnShip = 1ULL << 33,
    AlgorithmError_RejectedGoodContainer = 1ULL << 34,
    AlgorithmError_LoadAboveNotLegal = 1ULL << 35,
    AlgorithmError_UnloadNoContainersAtPosition = 1ULL << 36,
    AlgorithmError_UnloadBadId = 1ULL << 37,
    AlgorithmError_InvalidXYCoordinates = 1ULL << 38,

    // These are also kind of algorithm errors (they write the file)
    ReadOperations_InvalidFile = 1ULL << 40,
    ReadOperations_InsufficientRowData = 1ULL << 41,
    ReadOperations_InsufficientRowData_MoveOp = 1ULL << 42,
    ReadOperations_InvalidOperationType = 1ULL << 43,
    ReadOperations_InvalidShipPosition = 1ULL << 44,
    ReadOperations_reserved2 = 1ULL << 45,
    ReadOperations_reserved3 = 1ULL << 46,


};

class Error {
public:
    ErrorFlag errorFlag = ErrorFlag::Success;
    std::string errorMsg;
    std::string param1 = "<>";
    std::string param2 = "<>";
    std::string param3 = "<>";

    // region Constructors

    Error(ErrorFlag flag);

    Error(ErrorFlag errorFlag, const std::string &param1);

    Error(ErrorFlag errorFlag, const std::string &param1, const std::string &param2);

    Error(ErrorFlag errorFlag, const std::string &param1, const std::string &param2, const std::string &param3);

    Error(const std::string &errorMsg);

    Error(int errorFlags);

    // endregion

    // region Functions

    std::string toString();

    bool isFlag(ErrorFlag flag);  // kind of equality operator

    bool isFatalError();

    bool isAlgorithmError();

    bool isSuccess();

    // endregion
};

class Errors {
    std::vector<Error> errorsList;

public:

    // region Functions

    /// Adds an error to the list, only if its an actual error (i.e. contains message, or flag is not Success).
    void addError(const Error &error);

    StringVector toString() const;

    bool hasFatalError();

    bool hasAlgorithmErrors();

    /// Returns an int that represents all the errors in the list. If an error appears multiple times, it is treated as once.
    int toErrorFlag();

    bool hasNoErrors() const;

    bool hasErrors() const;
    // endregion

    // region Printer

    friend std::ostream &operator<<(std::ostream &os, const Errors &errors);
    // endregion

      // functions can use this as default parameter to ignore errors
};

// region Garbage Collector

extern Errors e_garbageCollector;

// endregion

#endif //SHIP_STOWAGE_MODEL_ERRORS_H
