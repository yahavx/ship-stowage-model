//
// Created by t-yabeny on 4/17/2020.
//

#ifndef SHIP_STOWAGE_MODEL_UTILFUNCTIONS_H
#define SHIP_STOWAGE_MODEL_UTILFUNCTIONS_H

#include <ostream>
#include "Definitions.h"


/// Prints a string vector.
std::ostream &operator<<(std::ostream &os, const StringVector &stringVector);

/// Prints 2-dimensional string vector.
std::ostream &operator<<(std::ostream &os, const StringStringVector &stringStringVector);

/// Prints an int vector.
std::ostream &operator<<(std::ostream &os, const IntVector &intVector);

/// Prints 2-dimensional int vector.
std::ostream &operator<<(std::ostream &os, const IntIntVector &intIntVector);

/// Prints 3-tuple.
std::ostream &operator<<(std::ostream &os, const std::tuple<int, int, int> &tup);

/// Trim leading and trailing whitespaces from a string (in place).
void trimWhitespaces(std::string &s);


#endif //SHIP_STOWAGE_MODEL_UTILFUNCTIONS_H
