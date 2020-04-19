//
// Created by t-yabeny on 4/17/2020.
//

#include "Printers.h"
#include <tuple>


std::ostream &operator<<(std::ostream &os, const StringVector &stringVector) {
    std::cout << '[';
    for (longUInt i = 0; i < stringVector.size() - 1; i++) {
        std::cout << stringVector[i] << ", \t\t";
    }
    std::cout << stringVector.back() << "]" << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, const StringStringVector &stringStringVector) {
    for (longUInt i = 0; i < stringStringVector.size(); i++) {
        std::cout << ' ' << stringStringVector[i];
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const IntVector &intVector) {
    std::cout << "\t[";
    for (longUInt i = 0; i < intVector.size() - 1; i++) {
        std::cout << intVector[i] << ", \t";
    }
    std::cout << intVector.back() << "]" << std::endl;
    return os;
}


std::ostream &operator<<(std::ostream &os, const IntIntVector &intIntVector) {
    for (longUInt i = 0; i < intIntVector.size(); i++) {
        std::cout << '\t' << intIntVector[i];
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::tuple<int, int, int> &tup) {
    os << "[";
    os << std::get<0>(tup) << ", " << std::get<1>(tup) << ", " << std::get<2>(tup);
    return os << "]";
}

std::ostream &operator<<(std::ostream &os, const OPS &ops) {
    if (ops.size() == 0) {
        std::cout << "<NoOperations>" << std::endl;
        return os;
    }
    std::cout << "Operations {" << std::endl;
    for (longUInt i = 0; i < ops.size(); i++) {
        std::cout << '\t' << ops[i] << std::endl;
    }
    std::cout << "}" << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, const StringToStringVectorMap &map) {
    std::cout << "StringToStringStringVector map {" << std::endl;
    for (auto key : map) {
        std::cout << "\t\"" << key.first << "\":" << std::endl;
        for (auto &val : key.second) {
            std::cout <<"\t\t" << val << std::endl;
        }
    }

    std::cout << "}" << std::endl;

    return os;
}


void printSeparator() {
    std::cout << std::endl << std::endl << "--------------------------------------" << std::endl << std::endl;
}