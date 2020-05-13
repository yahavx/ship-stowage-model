//
// Created by t-yabeny on 5/1/2020.
//

#include <iostream>
#include "Errors.h"
#include "UtilFunctions.h"


// region Functions

void Errors::addError(const Error &error) {
    if (error.errorFlag != ErrorFlag::Success || error.errorMsg != "") {  // actual error
        errorsList.push_back(error);
    }
}

void Errors::addErrors(const Errors &errors) {
    errorsList.insert(errorsList.end(), errors.errorsList.begin(), errors.errorsList.end());
}

StringVector Errors::toString() const {
    StringVector errors;
    for (Error error : errorsList) {
        errors.push_back(error.toString());
    }

    return errors;
}

int Errors::toErrorFlag() {
    int errors;
    for (Error error : errorsList) {
        if (error.errorFlag <= 1 << MAX_ERROR_BIT) {  // We want to collect only errors relevant to the algorithm
            errors |= error.errorFlag;
        }
    }

    return errors;
}

int Errors::toErrorFlagSinceLastCheckpoint() {
    int errors;
    for (longUInt i = checkpoint ; i < errorsList.size(); i++) {
        if (errorsList[i].errorFlag <= 1 << MAX_ERROR_BIT) {  // We want to collect only errors relevant to the algorithm
            errors |= errorsList[i].errorFlag;
        }
    }

    return errors;
}

int Errors::compareReports(int otherErrorFlag) {
    int selfFlag = this ->toErrorFlagSinceLastCheckpoint();

    for (int i = 0 ; i <= MAX_ERROR_BIT; i++) {
        bool errorAtSelf = (1 << i) & selfFlag;
        bool errorAtOther = (1 << i) & otherErrorFlag;
        if (errorAtSelf && !errorAtOther) {
            return i;
        }

        if (errorAtOther && !errorAtSelf) {
            return -i;
        }
    }

    return 0;
}


// endregion

// region Logging

void Errors::addSimulationInitLog() {
    addLog("The following errors were detected during the initialization of the Simulator and the Algorithm:");
}

void Errors::addSimulationPortVisitLog(int portVisitNum, const std::string &portId, int totalStops) {
    addLog("The following errors were detected on visit number " + intToStr(portVisitNum) + " in port '" + portId + "', which is stop number " +
           intToStr(totalStops) + " since the beginning of the journey:");
}

void Errors::addSimulationErrorLog() {
    addSeparator(errorsList.size());
    std::string logMsg = "The simulation has been terminated due to an algorithm error.";
    errorsList.push_back(logMsg);
}

void Errors::addTravelLog(const std::string &travelName) {
    addLog("The following errors were detected on travel '" + travelName +"':");
}

void Errors::addDynamicLoadErrorLog() {
    addLog("The following errors were detected during the algorithms dynamic loading:");
}

void Errors::addLog(const std::string &logMessage) {
    if (errorsList.size() > checkpoint) {
        errorsList.insert(errorsList.begin() + checkpoint, logMessage);
        if (checkpoint != 0) {
            addSeparator(checkpoint);
        }
        setCheckpoint();
    }
}

void Errors::addSeparator(int pos) {
    std::string separator = "-----------------------------------------------------------------";
    errorsList.insert(errorsList.begin() + pos, separator);
}

void Errors::setCheckpoint() {
    checkpoint = errorsList.size();
}

// endregion

// region Check Error Type

bool Errors::hasFatalError() {
    for (Error error : errorsList) {
        if (error.isFatalError())
            return true;
    }

    return false;
}

bool Errors::hasAlgorithmErrors() {


    for (Error &error : errorsList) {
        if (error.isAlgorithmError()) {
            return true;
        }
    }

    return false;
}

bool Errors::hasNoErrors() const {
    return errorsList.empty();
}

bool Errors::hasErrors() const {
    return !errorsList.empty();
}

// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const Errors &errors) {
    StringVector errorsStr = errors.toString();
    std::cout << "Errors {" << std::endl;
    for (longUInt i = 0; i < errorsStr.size(); i++) {
        std::cout << errorsStr[i] << std::endl;
    }
    std::cout << "}" << std::endl;
    return os;
}

// endregion