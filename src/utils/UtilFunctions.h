//
// Created by t-yabeny on 4/17/2020.
//

#ifndef SHIP_STOWAGE_MODEL_UTILFUNCTIONS_H
#define SHIP_STOWAGE_MODEL_UTILFUNCTIONS_H

#include <ostream>
#include "Definitions.h"


/// Trim leading and trailing whitespaces from a string (in place).
void trimWhitespaces(std::string &s);

/// Checks if string is an integer.
bool isInteger(const std::string &str);

/// Checks if 2-dimensional vector string contains only integers.
bool isDataOnlyIntegers(const StringStringVector &data);

/// Converts string to int. Doesn't validate its legal.
int stringToInt(const std::string &str);

/// Converts strings vector to ints vector. Doesn't validate the strings are actual numbers.
IntIntVector convertDataToInt(const StringStringVector &data);

/// Checks if word consists only of english letters (a-z, A-Z)
bool isEnglishWord(const std::string &str);



#endif //SHIP_STOWAGE_MODEL_UTILFUNCTIONS_H
