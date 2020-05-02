//
// Created by t-yabeny on 4/17/2020.
//

#ifndef SHIP_STOWAGE_MODEL_UTILFUNCTIONS_H
#define SHIP_STOWAGE_MODEL_UTILFUNCTIONS_H

#include <ostream>
#include <map>
#include "Definitions.h"

// region Type Conversions

/// Converts string to int. Doesn't validate its legal.
int stringToInt(const std::string &str);

/// Converts int to string.
std::string intToString(int num);

std::string craneOperationToString(const PackingOperation &op);

/// Converts a string vector to an int vector. Doesn't validate the strings are actual numbers.
IntVector convertRowToInt(const StringVector &row);

/// Converts 2-dim string vector to 2-dim ints vector. Doesn't validate the strings are actual numbers.
IntIntVector convertDataToInt(const StringStringVector &data);

/// Converts string representation of packing type to a packing type. Doesn't validate input.
PackingType packingTypeToString(char type);

/// Converts packing type to its string representation. Doesn't validate input.
std::string packingTypeFromString(PackingType type);
// endregion

// region Condition checks

/// Checks if string is an integer.
bool isInteger(const std::string &str);

/// Checks if a string vector contains only integers.
bool isRowOnlyIntegers(const StringVector &row);

/// Checks if 2-dimensional vector string contains only integers.
bool isDataOnlyIntegers(const StringStringVector &data);

/// Checks if word consists only of english letters (a-z, A-Z)
bool isEnglishWord(const std::string &str);
// endregion

// region Files

/**
 * Extracts file name from a path (C:/A/B/C/d.txt -> d.txt).
 * @param removeExtension removes the file extension if exists (.txt for example).
 */
std::string extractFilenameFromPath(const std::string &file, bool removeExtension);

/// Returns a list of files of a directory. If can't open directory, returns empty vector.
StringVector getFilesFromDirectory(const std::string &directoryPath);

/// Creates a folder, returns true if succeed.
bool createFolder(const std::string &path);

/// Removes a folder (and all its content).
bool removeFolder(const std::string &path);

/// Returns true if folder is empty.
bool isFolderEmpty(const std::string &path);

/// Creates empty file (assuming filePath is a file that doesn't exist, only its directory).
bool createEmptyFile(const std::string &filePath);

/// Return true if the path supplied is a directory.
bool isDirectoryExists(const std::string &directoryPath);

/// Return true if the path supplied is a file.
bool isFileExist(const std::string &filePath);
// endregion

// region Strings

/// Trim leading and trailing whitespaces from a string (in place).
void trimWhitespaces(std::string &s);

/// Returns the string in uppercase letters.
std::string toUpper(const std::string &str);

/// endWith string function.
bool endsWith(const std::string &str, const std::string &suffix);

/// startsWith string function.
bool startsWith(const std::string &str, const std::string &prefix);
// endregion

#endif //SHIP_STOWAGE_MODEL_UTILFUNCTIONS_H
