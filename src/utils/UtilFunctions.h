//
// Created by t-yabeny on 4/17/2020.
//

#ifndef SHIP_STOWAGE_MODEL_UTILFUNCTIONS_H
#define SHIP_STOWAGE_MODEL_UTILFUNCTIONS_H

#include <ostream>
#include "Definitions.h"


/// Prints a string vector.
std::ostream &operator<<(std::ostream &os, const StringVector &string);

/// Prints 2-dimensional string vector.
std::ostream &operator<<(std::ostream &os, const StringStringVector &string);

/// Trim leading and trailing whitespaces from a string (in place).
void trimWhitespaces(std::string &s);


#endif //SHIP_STOWAGE_MODEL_UTILFUNCTIONS_H
