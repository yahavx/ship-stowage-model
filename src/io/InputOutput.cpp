//
// Created by t-yabeny on 4/7/2020.
//

#include "InputOutput.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>

// basic read write example


bool SkipBOM(std::istream &in)  // some files contains garbage bytes at the start, this will eliminate them if needed
{
    char test[4] = {0};
    in.read(test, 3);
    if (strcmp(test, "\xEF\xBB\xBF") == 0)
        return true;
    in.seekg(0);
    return false;
}

std::vector<std::vector<std::string>> read_csv_file(std::string path) {
    std::fstream fin;

    fin.open(path, std::ios::in);
    std::vector<std::vector<std::string>> data;  // the data from the file will be saved as a matrix

    std::string line, word;

    SkipBOM(fin);  // we can ignore the return value

    while (getline(fin, line)) {  // reads a row
        data.emplace_back();  // adds a new empty vector (data row)

        if (line[0] == '#' || std::all_of(line.begin(), line.end(), isspace))  // skip lines with # or empty lines
            continue;

        std::stringstream s(line);  // used to split string to tokens

        while (getline(s, word, ',')) {  // get tokens one by one with ', ' delimiter
            std::cout << word;
            data.back().push_back(word);  // push word to current row
        }
        std::cout << " Current:";
        for (auto const& c : data.back())
            std::cout << c;
        std::cout << "try..";
        std::cout << data[0][0];
        std::cout << "end";

        std::cout << std::endl;
    }

    return data;
}

