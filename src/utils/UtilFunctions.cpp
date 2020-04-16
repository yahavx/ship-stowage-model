//
// Created by t-yabeny on 4/17/2020.
//

#include <iostream>
#include <algorithm>
#include "UtilFunctions.h"


std::ostream &operator<<(std::ostream &os, const StringVector &stringVector) {
    std::cout << '[';
    for (int i = 0; i < stringVector.size() - 1; i++){
        std::cout << stringVector[i] << ", \t\t";
    }
    std::cout << stringVector.back() << "]" << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, const StringStringVector &stringStringVector) {
    for (int i = 0; i < stringStringVector.size(); i++){
        std::cout << ' ' << stringStringVector[i];
    }
    return os;
}

void trimWhitespaces(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}
