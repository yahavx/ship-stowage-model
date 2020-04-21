//
// Created by t-yabeny on 4/7/2020.
//

#ifndef SHIP_STOWAGE_MODEL_FILEREADER_H
#define SHIP_STOWAGE_MODEL_FILEREADER_H

#include <string>
#include <vector>
#include "../../data_objects/ShipPlan.h"
#include "../../utils/Definitions.h"


/// Reads a file to a 2-dimension matrix, ignores lines that start with '#' and empty lines.
StringStringVector readFile(const std::string &path);

/**
 * Writes 2-dimension matrix to a file, a line per entry, tokens in a line split with commas.
 * @param path to write to, overwritten if exists.
 * @return true if completed successfully.
 */
bool writeFile(const std::string &path, const StringStringVector &data);

/// Checks if a file is in cargo format: ABCDE_<num>.cargo_data.
bool isCargoDataFileFormat(const std::string &fileName);

/// Returns true if the the path given is a directory.
bool isDirectoryExists(const std::string& directory);

/// Creates folder, returns true if succeed.
bool createFolder(const std::string &path);

#endif //SHIP_STOWAGE_MODEL_FILEREADER_H
