//
// Created by t-yabeny on 4/17/2020.
//

#ifndef SHIP_STOWAGE_MODEL_UTILFUNCTIONS_H
#define SHIP_STOWAGE_MODEL_UTILFUNCTIONS_H

#include <ostream>
#include <map>
#include "Definitions.h"


/// Trim leading and trailing whitespaces from a string (in place).
void trimWhitespaces(std::string &s);

/// Checks if string is an integer.
bool isInteger(const std::string &str);

/// Checks if 2-dimensional vector string contains only integers.
bool isDataOnlyIntegers(const StringStringVector &data);

/// Converts string to int. Doesn't validate its legal.
int stringToInt(const std::string &str);

/// Converts int to string.
std::string intToString(int num);

/// Converts strings vector to ints vector. Doesn't validate the strings are actual numbers.
IntIntVector convertDataToInt(const StringStringVector &data);

/// Checks if word consists only of english letters (a-z, A-Z)
bool isEnglishWord(const std::string &str);

/// endWith string function.
bool endsWith(const std::string &str, const std::string &suffix);

/// startsWith string function.
bool startsWith(const std::string &str, const std::string &prefix);

/// Converts string representation of packing type to a packing type. Doesn't validate input.
PackingType packingTypeToString(char type);

/// Converts packing type to its string representation. Doesn't validate input.
std::string packingTypeFromString(PackingType type);

/**
 * Extracts file name from a path (C:/A/B/C/d.txt -> d.txt).
 * @param removeExtension removes the file extension if exists (.txt for example).
 */
std::string extractFilenameFromPath(const std::string &file, bool removeExtension);

/// Returns the string in uppercase letters.
std::string toUpper(const std::string &str);

/// Creates a folder, returns true if succeed.
bool createFolder(const std::string &path);

std::string craneOperationToString(const PackingOperation &op);

/// Returns a list of files of a directory
StringVector getFilesFromDirectory(const std::string &directoryPath);

#endif //SHIP_STOWAGE_MODEL_UTILFUNCTIONS_H
