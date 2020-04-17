//
// Created by t-yabeny on 4/17/2020.
//

#include "Printers.h"
#include <tuple>


std::ostream &operator<<(std::ostream &os, const StringVector &stringVector) {
    std::cout << '[';
    for (int i = 0; i < stringVector.size() - 1; i++) {
        std::cout << stringVector[i] << ", \t\t";
    }
    std::cout << stringVector.back() << "]" << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, const StringStringVector &stringStringVector) {
    for (int i = 0; i < stringStringVector.size(); i++) {
        std::cout << ' ' << stringStringVector[i];
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const IntVector &intVector) {
    std::cout << '[';
    for (int i = 0; i < intVector.size() - 1; i++) {
        std::cout << intVector[i] << ", \t";
    }
    std::cout << intVector.back() << "]" << std::endl;
    return os;
}


std::ostream &operator<<(std::ostream &os, const IntIntVector &intIntVector) {
    for (int i = 0; i < intIntVector.size(); i++) {
        std::cout << ' ' << intIntVector[i];
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::tuple<int, int, int> &tup) {
    os << "[";
    os << std::get<0>(tup) << ", " << std::get<1>(tup) << ", " << std::get<2>(tup);
    return os << "]";
}