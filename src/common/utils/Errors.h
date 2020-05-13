//
// Created by t-yabeny on 5/1/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ERRORS_H
#define SHIP_STOWAGE_MODEL_ERRORS_H

#include <ostream>
#include "Definitions.h"

#define MAX_ERROR_BIT 18  // max that is used by the algorithm (rest are for us)
//#define RUNNING_ON_NOVA  // this will turn on all sections in code that work only on nova - disable note when on nova
#define SKIP_ISO_CHECK

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

    // our errors TODO: make sure the algorithm can't receive any by mistake
    SimulationInit_OutputDirectoriesCreationFailed = 1 << 21,
    SimulationInit_InvalidTravelPath = 1 << 22,
    SimulationInit_AllTravelsAreInvalid = 1ULL << 23,
    SimulationCleanup_OutputDirectoriesCleaningFailed = 1 << 24,

    Travel_InvalidDirectory = 1 << 25,
    Travel_FatalInput = 1 << 26,
    Travel_UnknownFile = 1 << 27,
    Travel_CargoData_PortNotInRoute = 1 << 28,
    Travel_CargoData_RemainingFilesAfterFinish = 1 << 29,

    AlgorithmError_CraneOperationWithInvalidId = 1 << 30,  // TODO: this should be mapped to error 32/33
    AlgorithmError_InvalidCraneOperation = 1ULL << 31,
    AlgorithmError_LeftContainersAtPort = 1ULL << 32,
    AlgorithmError_ContainerIdAlreadyOnShip = 1ULL << 33,
    AlgorithmError_ContainerIdNotExistsOnPort = 1ULL << 34,
    AlgorithmError_ContainerIdNotExistsOnShip = 1ULL << 35,
    AlgorithmError_RejectedGoodContainer = 1ULL << 36,
    AlgorithmError_LoadAboveNotLegal = 1ULL << 37,
    AlgorithmError_UnloadNoContainersAtPosition = 1ULL << 38,
    AlgorithmError_UnloadBadId = 1ULL << 39,
    AlgorithmError_UnloadBadPosition = 1ULL << 59,
    AlgorithmError_InvalidXYCoordinates = 1ULL << 40,
    AlgorithmError_MoveNoContainersAtPosition = 1ULL << 41,
    AlgorithmError_MoveBadId = 1ULL << 42,
    AlgorithmError_MoveAboveNotLegal = 1ULL << 43,
    AlgorithmError_TriedToLoadButShouldReject = 1ULL << 44,
    AlgorithmError_UnloadedAndDidntLoadBack = 1ULL << 45,
    AlgorithmError_ExtraReport = 1ULL << 46,
    AlgorithmError_MissingReport = 1ULL << 52,
    AlgorithmError_WeightBalancerRejectedOperation = 1ULL << 60,

    // These are also kind of algorithm errors (they write the file)
    ReadOperations_InvalidFile = 1ULL << 47,
    ReadOperations_InsufficientRowData = 1ULL << 48,
    ReadOperations_InsufficientRowData_MoveOp = 1ULL << 49,
    ReadOperations_InvalidOperationType = 1ULL << 50,
    ReadOperations_InvalidShipPosition = 1ULL << 51,
    ReadOperations_reserved2 = 1ULL,
    ReadOperations_reserved3 = 1ULL,

    SharedObject_CantLoadSoFile = 1ULL << 54,
    SharedObject_InvalidDirectory = 1ULL << 55,
    SharedObject_AlgorithmDidntSelfRegister = 1ULL << 56,
    SharedObject_LoadedMoreThanOneAlgorithm = 1ULL << 57,
    SharedObject_NoAlgorithmsLoaded = 1ULL << 58,
};

class Error {
public:
    ErrorFlag errorFlag = ErrorFlag::Success;
    std::string errorMsg;
    std::string param1 = "<>";
    std::string param2 = "<>";
    std::string param3 = "<>";
    std::string param4 = "<>";

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

    std::string toString();

    bool isFlag(ErrorFlag flag);  // kind of equality operator

    bool isFatalError();

    bool isAlgorithmError();

    bool isSuccess();

    // endregion
};

class Errors {
    std::vector<Error> errorsList;
    longUInt checkpoint = 0;

public:

    // region Functions

    /// Adds an error to the list, only if its an actual error (i.e. contains message, or flag is not Success).
    void addError(const Error &error);

    void addErrors(const Errors &errors);

    StringVector toString() const;

    /// Returns an int that represents all the errors in the list. If an error appears multiple times, it is treated as once.
    int toErrorFlag();

    /// Like the one above, but only takes into account errors since the last log (see logging section for reference).
    int toErrorFlagSinceLastCheckpoint();

    /**
     * Compares the errors list to the list of errors represented by the flag.
     * @return 0 if they are equal. i (greater than zero) if i'th bit is turned only onself. -i if i'th bit is turned only on other.
     */
    int compareReports(int otherErrorFlag);

    // region

    // region Logging

    /// If errors were added (checking against the last call to log functions), it will add an informative line about the simulation init.
    void addSimulationInitLog();

    /// If errors were added, it will add an informative line about the state of the simulation.
    void addSimulationPortVisitLog(int portVisitNum, const std::string &portId, int totalStops);

    void addSimulationErrorLog();

    /// If errors were added, it will add an informative line about the travel.
    void addTravelLog(const std::string &travelName);

    void addDynamicLoadErrorLog();

private:
    /// Adds a log message if errors were added since the last call.
    void addLog(const std::string &logMessage);

    /// Add a separator line to the error list.
    void addSeparator(int pos);

    /// Used to set a checkpoint, to be able later detect if errors were added in a period of time.
    void setCheckpoint();

    // endregion

    // region Check Error Type

public:
    bool hasFatalError();

    bool hasAlgorithmErrors();

    bool hasNoErrors() const;

    bool hasErrors() const;

    // endregion

    // region Printer

    friend std::ostream &operator<<(std::ostream &os, const Errors &errors);

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_ERRORS_H
