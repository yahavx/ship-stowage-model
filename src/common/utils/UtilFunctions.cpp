//
// Created by t-yabeny on 4/17/2020.
//

#include "UtilFunctions.h"
#include <algorithm>
#include <tuple>
#include <stdio.h>
#include <filesystem>

// region Type Conversions

int stringToInt(const std::string &str) {
    return std::stoi(str);
}

std::string intToString(int num) {
    return std::to_string(num);
}

std::string craneOperationToString(const PackingOperation &op) {
    std::string x = std::to_string(std::get<0>(op.getFromPosition()));
    std::string y = std::to_string(std::get<1>(op.getFromPosition()));
    std::string z = std::to_string(std::get<2>(op.getFromPosition()));
    auto opString = "Op('" + packingTypeFromString(op.getType())  + "', ContainerID=" + op.getContainerId() +
                    ", Position=[" + x + ", " + y + ", " + z +"]";

    return opString;
}

IntIntVector convertDataToInt(const StringStringVector &data) {

    IntIntVector intData;

    for (auto &currentRow : data) {
        intData.emplace_back();  // add new row entry
        for (auto &token : currentRow) {
            intData.back().push_back(stringToInt(token));  // add tokens to row
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

// region Conditions

bool isInteger(const std::string &str) {
    if (str == "-1") {
        return true;
    }
    return !str.empty() && std::find_if(str.begin(),
                                        str.end(), [](unsigned char c) { return !std::isdigit(c); }) == str.end();
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

bool endsWith(const std::string &str, const std::string &suffix) {
    return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

bool startsWith(const std::string &str, const std::string &prefix) {
    return str.rfind(prefix, 0) == 0;
}
// endregion

// region Files

/**
 * Returns a file name.
 * @param path a full path to a file.
 * @param removeExtension remove the file extension, if exists (.txt for example)
 */
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

/// Returns list of files (and folders) in a directory (full path to each).
StringVector getFilesFromDirectory(const std::string &directoryPath) {
    StringVector stringVector;

    for (auto &tmp_directory : std::filesystem::directory_iterator(directoryPath)) {
        std::ostringstream myObjectStream; // build a stream
        myObjectStream << tmp_directory;  // write current file to a stream
        auto path = myObjectStream.str();  // pull the result from the stream
        path = path.substr(1, path.size() - 2);  // trim the "" from both sides
        stringVector.push_back(path);
    }

    return stringVector;
}
// endregion

// region Strings

std::string toUpper(const std::string &str) {
    std::string strCopy = str;
    std::transform(strCopy.begin(), strCopy.end(), strCopy.begin(), ::toupper);
    return strCopy;
}

/// Trim whitespaces from both sides.
void trimWhitespaces(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}
// endregion