//
// Created by t-yabeny on 5/18/2020.
//

#ifndef SHIP_STOWAGE_MODEL_TRACER_H
#define SHIP_STOWAGE_MODEL_TRACER_H

enum class TraceVerbosity {
    Fatal = 1, Warning = 2, Info = 3, Verbose = 4
};

class Tracer {

    TraceVerbosity verbosity;

public:

    Tracer(TraceVerbosity verbosity);

    // region Tracing

    /**
     * Trace with verbose level.
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
