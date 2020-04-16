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

bool SkipBOM(std::istream &in)  // some files contains garbage bytes at the start, this will eliminate them if needed
{
    char test[4] = {0};
    in.read(test, 3);
    if (strcmp(test, "\xEF\xBB\xBF") == 0)  // TODO: validate that strcmp is really from cstring
        return true;
    in.seekg(0);
    return false;
}

StringStringVector readFile(const std::string &path) {
    std::fstream fin;

    fin.open(path, std::ios::in);
    StringStringVector data;  // the data from the file will be saved as a matrix

    std::string line, word;

    SkipBOM(fin);  // we can ignore the return value

    while (getline(fin, line)) {  // reads a row
        if (line[0] == '#' || std::all_of(line.begin(), line.end(), isspace))  // skip lines with # or empty lines
            continue;

        data.emplace_back();  // adds a new empty vector (data row)

        std::stringstream s(line);  // used to split string to tokens

        while (getline(s, word, ',')) {  // get tokens one by one with ', ' delimiter
            trimWhitespaces(word);
            data.back().push_back(word);  // push word to current row
        }
    }

    return data;
}

bool isInteger(const std::string &str) {
    return !str.empty() && std::find_if(str.begin(),
                                        str.end(), [](unsigned char c) { return !std::isdigit(c); }) == str.end();
}

int stringToInt(const std::string &str) {
    return std::stoi(str);
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

bool isEnglishWord(const std::string &str) {
    for (char letter : str) {
        if (!((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'))) {
            return false;
        }
    }

    return true;
}