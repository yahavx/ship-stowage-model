//
// Created by t-yabeny on 5/18/2020.
//

#include <iostream>
#include "Tracer.h"
#include "../utils/Printers.h"


Tracer::Tracer(TraceVerbosity verbosity) : verbosity(verbosity) {}

// region Tracing

void Tracer::traceVerbose(const std::string &msg, bool separator) {

    trace(TraceVerbosity::Verbose, msg, separator);
}

void Tracer::traceInfo(const std::string &msg, bool separator) {
    trace(TraceVerbosity::Info, msg, separator);
}

void Tracer::traceWarning(const std::string &msg, bool separator) {
    trace(TraceVerbosity::Warning, msg, separator);
}

void Tracer::traceFatal(const std::string &msg, bool separator) {
    trace(TraceVerbosity::Fatal, msg, separator);
}

// endregion

void Tracer::separator(TraceVerbosity verbosityLevel, int linesBefore, int linesAfter) {
    if (this->verbosity >= verbosityLevel) {
        printSeparator(linesBefore, linesAfter);
    }
}

void Tracer::trace(TraceVerbosity verbosityLevel, const std::string &msg, bool separator) {
    #ifdef DEBUG_PRINTS

    if (this->verbosity >= verbosityLevel) {
        std::cout << msg << std::endl;

        if (separator)
            this->separator(verbosityLevel, 0, 0);
    }

    #endif
}

