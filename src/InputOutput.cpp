//
// Created by t-yabeny on 4/7/2020.
//

#include "InputOutput.h"
#include <fstream>
#include <iostream>
#include <string>

// basic read write example

void write(std::string data) {
    std::ofstream file;
    file.open("..\\example.txt");
    if (file.is_open()) {
        file << data << std::endl;
        file.close();
    } else {
        std::cout << "Unable to open file";
    }
}

void read() {
    std::string line;
    std::ifstream file;
    int i = 0;
    file.open("..\\example.txt");
    if (file.is_open()) {
        while (getline(file, line)) {
            std::cout << "line " << i << ": " << line << '\n';
        }
        file.close();
    } else {
        std::cout << "Unable to open file";
    }
}