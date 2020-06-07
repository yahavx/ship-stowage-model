//
// Created by t-yabeny on 5/1/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ERRORS_H
#define SHIP_STOWAGE_MODEL_ERRORS_H

#include <ostream>
#include "Definitions.h"
#include "Error.h"
#include "../loggers/Tracer.h"

//#define RUNNING_ON_NOVA  // this will turn on all sections in code that work only on nova - disable note when on nova
//#define SOFT_ISO_CHECK  // only check that ID is 11 characters long
#define DEBUG_PRINTS  // print logs during simulation
#define _unused(x) ((void)(x))  // ignore compiler warning


/**
 * This class represents a collection of errors.
 * It allows separating them to sections, logging, and convert them to a convenient string representation.
 */
class Errors {
    std::vector<Error> errorsList;
    longUInt checkpoint = 0;
    Tracer tracer;

public:

    Errors() = default;

    Errors(Tracer &tracer);

    // region Functions

    /// Adds an error to the list, only if its an actual error (i.e. contains message, or flag is not Success).
    void addError(const Error &error);

    /**
     * Adds a report about a list of errors.
     * @param report represents a list of errors.
     * @param reporter the name of the reporter (algorithm, simulator for example).
     */
    void addErrorReport(longUInt report, const std::string &reporter);

    void addErrors(const Errors &errors);

    StringVector toString() const;

    /**
     *  Returns an int that represents all the errors in the list. If an error appears multiple times, it is treated as once. Collects errors that are up to MAX_ERROR_BIT.
     *  @param limitErrorNum limits errors collected to be from 0 to MAX_ERROR_BIT.
     *  @param sinceLastCheckPoint only collect errors since the last checkpoint (see logging section for reference)
     */
    longUInt toErrorFlag(bool limitErrorNum = true, bool sinceLastCheckPoint = false);

//    /**
//     * Compares the errors list to the list of errors represented by the flag.
//     * @return 0 if they are equal. i (greater than zero) if i'th bit is turned only onself. -i if i'th bit is turned only on other.
//     */
//    int compareReports(int otherErrorFlag);

    // region

    // region Logging

    /// If errors were added (checking against the last call to log functions), it will add an informative line about the simulator init.
    void addSimulatorInitLog();

    /// Same as above, but for a simulation init (travel-algorithm pair).
    void addSimulationInitLog();

    /// If errors were added, it will add an informative line about the state of the simulation.
    void addSimulationPortVisitLog(int portVisitNum, const std::string &portId, int totalStops);

    void addSimulationErrorLog();

    void addSimulationFinishLog();

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
