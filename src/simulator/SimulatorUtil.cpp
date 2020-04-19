//
// Created by t-yabeny on 4/19/2020.
//

#include "SimulatorUtil.h"
#include "../utils/Definitions.h"
#include "../utils/UtilFunctions.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <filesystem>
#include "../utils/Printers.h"
#include "../common/io/FileReader.h"


// region sortTravelCargoData

/// Returns a list of files of a directory
StringVector getFilesFromDirectory(const std::string &directoryPath) {
    StringVector stringVector;

    for (auto &tmp_directory : std::filesystem::directory_iterator(directoryPath)) {
        std::ostringstream myObjectStream; // a stream is built
        myObjectStream << tmp_directory;  // write file to stream
        stringVector.push_back(myObjectStream.str());  // pull result from stream
    }

    return stringVector;
}


/// Sorts a string vector of .cargo_files by their numbers. Names must be valid, and all belong to each port.
void sortCargoFilesByNumber(StringVector &stringVector) {
    sort(stringVector.begin(), stringVector.end(),
         [](const std::string &a, const std::string &b) -> bool {
             std::string numA = a.substr(6, a.length() - 17);  // dirty trick to extract the number
             std::string numB = b.substr(6, b.length() - 17);
             return stringToInt(numA) < stringToInt(numB);
         });
}


StringToStringVectorMap sortTravelCargoData(const std::string &directoryPath) {
    StringToStringVectorMap map;

    StringVector files = getFilesFromDirectory(directoryPath);

    for (std::string file : files) {
        std::string fileName = extractFilenameFromPath(file, false);
        fileName.pop_back();  // trim " from the end

        if (!isCargoDataFileFormat(fileName)) {
            if (fileName == "Route" || fileName == "Plan") {
                continue;  // we except to see this, need to just ignore
            }

            std::cout << "Warning: invalid file in travel folder: " << fileName << std::endl;
            continue;
        }

        std::string portId = fileName.substr(0, 5);

        if (map.find(portId) == map.end()) {  // port not encountered yet
            map[portId] = StringVector();
        }

        map[portId].push_back(fileName);  // we push in correct order because files are sorted
    }

    // now we have mapping from AAAAA -> AAAAA_17, AAAAA_2 (unsorted because 17 < 2 but alphabetically 2 < 17)
    for (auto &entry: map) {
        sortCargoFilesByNumber(entry.second);  // sort for each port separately
    }

    return map;
}
// endregion