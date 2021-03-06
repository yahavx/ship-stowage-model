//
// Created by t-yabeny on 4/7/2020.
//

#include "FileReader.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <sstream>
#include "../utils/UtilFunctions.h"
#include <string>
#include <filesystem>


bool SkipBOM(std::istream &in)  // some files contains garbage bytes at the start, this will eliminate them if needed
{
    char test[4] = {0};
    in.read(test, 3);
    if (strcmp(test, "\xEF\xBB\xBF") == 0)
        return true;
    in.seekg(0);
    return false;
}

StringStringVector readFile(const std::string &path) {
    std::fstream fin;
    StringStringVector data;  // the data from the file will be saved as a matrix

    fin.open(path, std::ios::in);
    if (!fin) {  // couldn't open file
        return data;
    }

    std::string line, word;

    SkipBOM(fin);  // we can ignore the return value

    while (getline(fin, line)) {  // reads a row
        data.emplace_back();  // adds a new empty vector (data row), may be empty

        if (line[0] == '#' || startsWith(line, "\xEF\xBB\xBF#") || std::all_of(line.begin(), line.end(), isspace))  // skip lines with # or empty lines
            continue;

        std::stringstream s(line);  // used to split line to tokens

        while (getline(s, word, ',')) {  // get tokens one by one with ', ' delimiter
            trimWhitespaces(word);
            data.back().push_back(word);  // push word to current row
        }
    }

    return data;
}

bool writeFile(const std::string &path, const StringStringVector &data) {
    std::ofstream outputFile(path);

    if (!outputFile) {
        return false;
    }

    for (StringVector dataRow : data) {
        for (longUInt i = 0; i < dataRow.size() - 1; i++) {
            outputFile << dataRow[i] << ", ";
        }
        outputFile << dataRow.back() << std::endl;
    }

    return true;
}

bool writeFile(const std::string &path, const StringVector &data) {
    std::ofstream outputFile(path);

    if (!outputFile) {
        return false;
    }

    for (std::string row : data) {
        outputFile << row << std::endl;
    }

    return true;
}




