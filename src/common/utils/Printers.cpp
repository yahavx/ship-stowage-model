//
// Created by t-yabeny on 4/17/2020.
//

#include "Printers.h"
#include "Errors.h"
#include "UtilFunctions.h"
#include <tuple>


std::ostream &operator<<(std::ostream &os, const StringVector &stringVector) {
    if (stringVector.size() == 0) {
        os << "[]" << std::endl;
        return os;
    }

    std::cout << '[';
    for (longUInt i = 0; i < stringVector.size() - 1; i++) {
        os << stringVector[i] << ", \t\t";
    }
    os << stringVector.back() << "]" << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, const StringStringVector &stringStringVector) {
    for (longUInt i = 0; i < stringStringVector.size(); i++) {
        os << ' ' << stringStringVector[i];
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const IntVector &intVector) {
    os << "\t[";
    for (longUInt i = 0; i < intVector.size() - 1; i++) {
        os << intVector[i] << ", \t";
    }
    os << intVector.back() << "]" << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, const IntIntVector &intIntVector) {
    for (longUInt i = 0; i < intIntVector.size(); i++) {
        os << '\t' << intIntVector[i];
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::tuple<int, int, int> &tup) {
    os << "[";
    os << std::get<0>(tup) << ", " << std::get<1>(tup) << ", " << std::get<2>(tup);
    return os << "]";
}

std::ostream &operator<<(std::ostream &os, const StringToStringVectorMap &map) {
    os << "StringToStringStringVector map {" << std::endl;
    for (auto key : map) {
        os << "\t\"" << key.first << "\":" << std::endl;
        for (auto &val : key.second) {
            os <<"\t\t" << val << std::endl;
        }
    }

    os << "}" << std::endl;

    return os;
}

void printSeparator(int linesBefore, int linesAfter) {
#ifdef DEBUG_PRINTS
    for (int i = 0; i < linesBefore; i++) {
        std::cout << std::endl;
    }
    std::cout << "--------------------------------------" << std::endl;
    for (int i = 0; i < linesAfter; i++) {
        std::cout << std::endl;
    }
#endif

    _unused(linesBefore);
    _unused(linesAfter);
}

void printEmptyLines(int amount) {
    for (int i = 0; i < amount; i++) {
        std::cout << std::endl;
    }
}

void printErrorsFromFlag(int errorsFlag) {
    std::string errors = Error(errorsFlag).toString();
    std::cout << errors << std::endl;
}

std::string positionToString(int x, int y) {
    return "(" + intToStr(x) + ", " + intToStr(y) + ")";
}