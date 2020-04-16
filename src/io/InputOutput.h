//
// Created by t-yabeny on 4/7/2020.
//

#ifndef SHIP_STOWAGE_MODEL_INPUTOUTPUT_H
#define SHIP_STOWAGE_MODEL_INPUTOUTPUT_H

#include <string>
#include <vector>
#include "../data_objects/ShipPlan.h"


typedef std::vector<int> IntVector;
typedef std::vector<std::vector<int>> IntIntVector;
typedef std::vector<std::string> StringVector;
typedef std::vector<std::vector<std::string>> StringStringVector;

/// Reads a .csv or .txt file to a 2-dimension matrix, ignores lines that start with '#' and empty lines.
StringStringVector readFile(const std::string &path);

/// Checks if string is an integer.
bool isInteger(const std::string &str);

/// Checks if 2-dimensional vector string contains only integers.
bool isDataOnlyIntegers(const StringStringVector &data);

/// Converts string to int. Doesn't validate its legal.
int stringToInt(const std::string &str);

/// Converts strings vector to ints vector. Doesn't validate the strings are actual numbers.
IntIntVector convertDataToInt(const StringStringVector &data);

#endif //SHIP_STOWAGE_MODEL_INPUTOUTPUT_H
