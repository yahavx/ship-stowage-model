//
// Created by yahav on 13/05/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ERROR_H
#define SHIP_STOWAGE_MODEL_ERROR_H

#include "Definitions.h"

#define ALGORITHM_MAX_ERROR 18  // highest bit that is used by the algorithm (rest are for us)


/// Each entry represents a possible error/warning in the program.
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
    ContainersAtPort_ContainerNotOnRoute = 1 << 19,
    ContainersAtPort_ContainerDestinationIsCurrentPort = 1 << 20,

    // our errors
    SimulationInit_OutputDirectoriesCreationFailed = 1 << 21,
    SimulationInit_InvalidTravelPath = 1 << 22,
    SimulationInit_AllTravelsAreInvalid = 1ULL << 23,
    SimulationCleanup_OutputDirectoriesCleaningFailed = 1 << 24,

    Travel_InvalidDirectory = 1 << 25,
    Travel_FatalInput = 1 << 26,
    Travel_UnknownFile = 1 << 27,
    Travel_CargoData_PortNotInRoute = 1 << 28,
    Travel_CargoData_RemainingFilesAfterFinish = 1 << 29,

    AlgorithmError_CraneOperationWithInvalidId = 1 << 30,
    AlgorithmError_InvalidCraneOperation = 1ULL << 31,
    AlgorithmError_LeftContainersAtPort = 1ULL << 32,
    AlgorithmError_ContainerIdAlreadyOnShip = 1ULL << 33,
    AlgorithmError_ContainerIdNotExistsOnPort = 1ULL << 34,
    AlgorithmError_ContainerIdNotExistsOnShip = 1ULL << 35,
    AlgorithmError_RejectedGoodContainer = 1ULL << 36,
    AlgorithmError_LoadAboveNotLegal = 1ULL << 37,
    AlgorithmError_UnloadNoContainersAtPosition = 1ULL << 38,
    AlgorithmError_UnloadBadId = 1ULL << 39,
    AlgorithmError_UnloadBadPosition = 1ULL << 40,
    AlgorithmError_InvalidXYCoordinates = 1ULL << 41,
    AlgorithmError_MoveNoContainersAtPosition = 1ULL << 42,
    AlgorithmError_MoveBadId = 1ULL << 43,
    AlgorithmError_TriedToLoadButShouldReject = 1ULL << 44,
    AlgorithmError_MoveAboveNotLegal = 1ULL << 45,
    AlgorithmError_UnloadedAndDidntLoadBack = 1ULL << 46,
    AlgorithmError_LoadInvalidFloor = 1ULL << 47,
//    AlgorithmError_MissingReport = 1ULL << 48,
    AlgorithmError_ShipNotEmptyAtEndOfRoute = 1ULL << 48,
    AlgorithmError_WeightBalancerRejectedOperation = 1ULL << 49,
    AlgorithmError_FailedToInitialize = 1ULL << 50,

    // These are also kind of algorithm errors (they write the file)
    ReadOperations_InvalidFile = 1ULL << 51,
    ReadOperations_InsufficientRowData = 1ULL << 52,
    ReadOperations_InsufficientRowData_MoveOp = 1ULL << 53,
    ReadOperations_InvalidOperationType = 1ULL << 54,
    ReadOperations_InvalidShipPosition = 1ULL << 55,

    FileInput_TooManyParameters = 1ULL << 56,  // This is a general error for all inputs (plan, route, cargo data, operations)

    SharedObject_CantLoadSoFile = 1ULL << 57,
    SharedObject_InvalidDirectory = 1ULL << 58,
    SharedObject_AlgorithmDidntSelfRegister = 1ULL << 59,
    SharedObject_LoadedMoreThanOneAlgorithm = 1ULL << 60,
    SharedObject_NoAlgorithmsLoaded = 1ULL << 61,

    AlgorithmError_LeftContainersAtShip = 1ULL << 62,
};

/**
 *  Error class represents a single error.
 *  It is usually represented as an ErrorFlag (which can also have parameters to give more indicative error),
 *  but can also hold a custom message, in particular it allows logging messages as well.
 */
class Error {
public:
    // A custom error message
    std::string errorMsg;

    // A formatted error message - combine of an error flag with params (to give more indicative error - params can be port, container ID, line number, etc)
    ErrorFlag errorFlag = ErrorFlag::Success;
    std::string param1 = "<>";
    std::string param2 = "<>";
    std::string param3 = "<>";
    std::string param4 = "<>";

    // Combination of few error flags
    longUInt errors = 0;

    // region Constructors

    Error(ErrorFlag flag);

    Error(ErrorFlag errorFlag, const std::string &param1);

    Error(ErrorFlag errorFlag, const std::string &param1, const std::string &param2);

    Error(ErrorFlag errorFlag, const std::string &param1, const std::string &param2, const std::string &param3);

    Error(ErrorFlag errorFlag, const std::string &param1, const std::string &param2, const std::string &param3, const std::string &param4);

    Error(const std::string &errorMsg);

    Error(int errorFlags);

    // endregion

    // region Functions

    std::string toString() const;

    bool isCertainFlag(ErrorFlag other);  // kind of lazy equality operator

    bool isFatalError();

    bool isAlgorithmError();

    bool isSuccess();

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_ERROR_H
