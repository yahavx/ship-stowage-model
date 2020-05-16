//
// Created by t-yabeny on 5/2/2020.
//

#ifndef SHIP_STOWAGE_MODEL_FILETESTS_H
#define SHIP_STOWAGE_MODEL_FILETESTS_H

#include <string>
#include "../src/common/utils/Definitions.h"
#include "../src/common/utils/UtilFunctions.h"
#include "../src/common/utils/Printers.h"

using namespace std;

void inline isFileOrDirectory() {
    std::string path = "../main.cpp";
    bool isExists = isFileExist(path);
    bool isExists2 = isDirectoryExists(path);
    StringVector files = getFilesFromDirectory(path);
    cout << files;

    cout << "Files number: " << files.size() << endl;
    cout << "Is file exist: " << isExists << endl;
    cout << "Is directory exist: " << isExists2;
};
#endif //SHIP_STOWAGE_MODEL_FILETESTS_H
