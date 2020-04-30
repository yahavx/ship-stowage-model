//
// Created by t-yabeny on 4/17/2020.
//

#ifndef SHIP_STOWAGE_MODEL_PRINTERS_H
#define SHIP_STOWAGE_MODEL_PRINTERS_H

#include <iostream>
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

/// Prints an operations vector.
std::ostream &operator<<(std::ostream &os, const OPS &ops);

/// Prints a map from string to StringStringVector.
std::ostream &operator<<(std::ostream &os, const StringToStringVectorMap &map);

/**
 * Prints a separator line (of dashes).
 * @param linesBefore number of line spaces before
 * @param linesAfter number of line spaces after
 */
void printSeparator(int linesBefore, int linesAfter);

/// Prints empty lines.
void printEmptyLines(int amount);


#endif //SHIP_STOWAGE_MODEL_PRINTERS_H
