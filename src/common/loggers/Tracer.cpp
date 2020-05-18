//
// Created by t-yabeny on 5/18/2020.
//

#include <iostream>
#include "Tracer.h"
#include "../utils/Printers.h"


Tracer::Tracer(TraceVerbosity verbosity) : verbosity(verbosity) {}

// region Tracing

void Tracer::traceVerbose(const std::string &msg) {

    if (this->verbosity >= TraceVerbosity::Verbose) {
        print(msg);
    }
}

void Tracer::traceInfo(const std::string &msg) {
    if (this->verbosity >= TraceVerbosity::Info) {
        print(msg);
    }
}

void Tracer::traceWarning(const std::string &msg) {
    if (this->verbosity >= TraceVerbosity::Warning) {
        print(msg);
    }
}

void Tracer::traceFatal(const std::string &msg) {
    if (this->verbosity >= TraceVerbosity::Fatal) {
        print(msg);
    }
}

// endregion

void Tracer::separator(TraceVerbosity verbosityLevel, int linesBefore, int linesAfter) {
    if (this->verbosity >= verbosityLevel) {
        printSeparator(linesBefore, linesAfter);
    }
}

void Tracer::print(const std::string &msg) {
    #ifdef DEBUG_PRINTS
    std::cout << msg << std::endl;
    #endif
}

