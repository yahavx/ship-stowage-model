//
// Created by t-yabeny on 4/17/2020.
//

#include "UtilFunctions.h"
#include <algorithm>
#include <tuple>
#include <stdio.h>
#include <filesystem>
#include <fstream>

// region Type Conversions

int strToInt(const std::string &str) {
    return std::stoi(str);
}

std::string intToStr(int num) {
    return std::to_string(num);
}

std::string craneOperationToString(const PackingOperation &op) {
    std::string x = std::to_string(std::get<0>(op.getFirstPosition()));
    std::string y = std::to_string(std::get<1>(op.getFirstPosition()));
    std::string z = std::to_string(std::get<2>(op.getFirstPosition()));
    auto opString = "Op('" + packingTypeFromString(op.getType()) + "', ContainerID=" + op.getContainerId() +
                    ", Position=[" + x + ", " + y + ", " + z + "]";

    return opString;
}

IntVector convertRowToInt(const StringVector &row) {
    IntVector intRow;

    for (auto &token : row) {
        intRow.push_back(strToInt(token));  // add tokens to row
    }

    return intRow;
}

IntIntVector convertDataToInt(const StringStringVector &data) {

    IntIntVector intData;

    for (auto &currentRow : data) {
        intData.emplace_back();  // add new row entry
        for (auto &token : currentRow) {
            intData.back().push_back(strToInt(token));  // add tokens to row
        }
    }

    return intData;
}

PackingType packingTypeToString(char type) {
    switch (type) {
        case 'L':
            return PackingType::load;
        case 'U':
            return PackingType::unload;
        case 'M':
            return PackingType::move;
        default:  // case 'R'
            return PackingType::reject;
    }
}

std::string packingTypeFromString(PackingType type) {
    switch (type) {
        case PackingType::load:
            return "L";
        case PackingType::unload:
            return "U";
        case PackingType::move:
            return "M";
        default:  // case PackingType::reject
            return "R";
    }
}
// endregion

// region Condition checks

bool isInteger(const std::string &str) {
    return !str.empty() && std::find_if(str.begin(),
                                        str.end(), [](unsigned char c) { return !std::isdigit(c); }) == str.end();
}

bool isRowOnlyIntegers(const StringVector &row) {
    for (auto &token : row) {
        if (!isInteger(token))
            return false;
    }
    return true;
}

bool isDataOnlyIntegers(const StringStringVector &data) {
    for (auto &dataRow : data) {
        for (auto &token : dataRow) {
            if (!isInteger(token))
                return false;
        }
    }
    return true;
}

bool isEnglishWord(const std::string &str) {
    for (char letter : str) {
        if (!((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'))) {
            return false;
        }
    }

    return true;
}

bool isLegalCategoryIdentifier(const std::string &str) {
    return str == "U" || str == "J" || str == "Z";
}
// endregion

// region Files

std::string extractFilenameFromPath(const std::string &path, bool removeExtension) {
    std::string pathCopy = path;  // it works inplace so we create a copy
    // Remove directory if present.
    // Do this before extension removal incase directory has a period character.
    const size_t last_slash_idx = pathCopy.find_last_of("\\/");
    if (std::string::npos != last_slash_idx) {
        pathCopy.erase(0, last_slash_idx + 1);
    }

    // Remove extension if present.
    if (removeExtension) {
        const size_t period_idx = pathCopy.rfind('.');
        if (std::string::npos != period_idx) {
            pathCopy.erase(period_idx);
        }
    }

    return pathCopy;
}

StringVector getFilesFromDirectory(const std::string &directoryPath) {
    StringVector files;

    if (!isDirectoryExists(directoryPath)) {
        return files;
    }

    for (auto &tmp_directory : std::filesystem::directory_iterator(directoryPath)) {
        std::ostringstream myObjectStream; // build a stream
        myObjectStream << tmp_directory;  // write current file to a stream
        auto path = myObjectStream.str();  // pull the result from the stream
        path = path.substr(1, path.size() - 2);  // trim the "" from both sides
        files.push_back(path);
    }

    return files;
}

bool createFolder(const std::string &path) {
    if (isDirectoryExists(path)) {  // if its already created, its also okay
        return true;
    }

    try {
        std::filesystem::create_directory(path);
        return true;
    }
    catch (...) {  // failed
        return false;
    }
}

bool removeFolder(const std::string &path) {
    return std::filesystem::remove_all(path);
}

bool isFolderEmpty(const std::string &path) {
    StringVector files = getFilesFromDirectory(path);
    return files.empty();
}

bool createEmptyFile(const std::string &filePath) {
    std::ofstream outputFile(filePath);
    if (!outputFile) { // couldn't open
        return false;
    }

    outputFile << "";
    outputFile.close();
    return true;
}

bool isDirectoryExists(const std::string& directory)
{
    return std::filesystem::is_directory(directory);
}

bool isFileExist(const std::string &filePath)
{
    std::ifstream infile(filePath);
    return infile.good();
}

// endregion

// region Strings

std::string toUpper(const std::string &str) {
    std::string strCopy = str;
    std::transform(strCopy.begin(), strCopy.end(), strCopy.begin(), ::toupper);
    return strCopy;
}

void trimWhitespaces(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

bool endsWith(const std::string &str, const std::string &suffix) {
    return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

bool startsWith(const std::string &str, const std::string &prefix) {
    return str.rfind(prefix, 0) == 0;
}
// endregion