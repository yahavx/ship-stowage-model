//
// Created by t-yabeny on 5/18/2020.
//

#ifndef SHIP_STOWAGE_MODEL_TRACER_H
#define SHIP_STOWAGE_MODEL_TRACER_H

#include <fstream>

enum class TraceVerbosity {
    Disabled = 0, Fatal = 1, Warning = 2, Info = 3, Verbose = 4
};

class Tracer {

    TraceVerbosity verbosity;  // Handle traces with this verbosity or higher.

public:

    // region Constructors

    Tracer();  // Disabled tracer

    Tracer(TraceVerbosity verbosity);

    // endregion

    // region Tracing

    /**
     * Trace with verbose (lowest) level.
     * @param msg message to trace.
     * @param separator add separator after.
     */
    void traceVerbose(const std::string &msg, bool separator = false);

    void traceInfo(const std::string &msg, bool separator = false);

    void traceWarning(const std::string &msg, bool separator = false);

    void traceFatal(const std::string &msg, bool separator = false);

    // endregion

    void separator(TraceVerbosity verbosityLevel, int linesBefore, int linesAfter);

private:

    void trace(TraceVerbosity verbosityLevel, const std::string &msg, bool separator);
};


#endif //SHIP_STOWAGE_MODEL_TRACER_H
