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

    void traceVerbose(const std::string &msg);

    void traceInfo(const std::string &msg);

    void traceWarning(const std::string &msg);

    void traceFatal(const std::string &msg);

    // endregion

    void separator(TraceVerbosity verbosityLevel, int linesBefore, int linesAfter);

private:

    void print(const std::string &msg);
};


#endif //SHIP_STOWAGE_MODEL_TRACER_H
