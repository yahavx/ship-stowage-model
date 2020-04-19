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
#include "../algorithms/stowage/IStowageAlgorithm.h"
#include "Simulator.h"
#include "../common/io/ObjectsReader.h"

static std::string staticOutputFile =  "../simulation-output/cargo_instructions";  // TODO: take it from the simulator class somehow!
static std::string unloadOnly = "UnloadOnly:";

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
                continue;  // we except to see this, so just ignore
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

std::optional<std::string> getNextFileForPort(StringToStringVectorMap &map, const std::string &portId) {
    if (map.find(portId) == map.end()) {  // this port does not exist
        return std::nullopt;
    }

    StringVector &filesForPort = map[portId];

    if (filesForPort.size() == 0) {  // no files remaining
        return std::nullopt;
    }

    std::string first = filesForPort[0];  // retrieve first element
    filesForPort.erase(filesForPort.begin());  // pop first element
    return first;
}

void filterUnusedPorts(StringToStringVectorMap &map, ShipRoute &shipRoute) {
    StringVector toErase;

    for (auto &entry: map) {
        std::string currPortCode = entry.first;  // get a port id

        bool found = false;  // indicates if we found this port in the route
        for (const PortId &portId : shipRoute.getPorts()) {
            if (portId.getCode() == currPortCode) {
                found = true;
                break;
            }
        }

        if (!found) {  // port is not in the route, remove it
            std::cout << "Warning: port " << currPortCode << " has cargo files but doesn't appear in the route, ignoring" << std::endl;
            toErase.push_back(currPortCode);  // we don't erase in-place because it will crash the map iterator
        }
    }

    for (std::string& port : toErase){
        map.erase(port);
    }
}

void filterTwiceInARowPorts(ShipRoute &shipRoute) {
    StringVector toErase;
    std::string last = "";
    std::vector<PortId> newPorts;

    for (PortId portId : shipRoute.getPorts()) {
        if (portId.getCode() == last)
            continue;
        last = portId.getCode();
        newPorts.push_back(PortId(portId));
    }
    shipRoute.setPorts(newPorts);  // we don't if we didn't change anything also, but whatever
}

std::string getCargoPath(const std::string &travel, const std::string &cargoFile) {
    return travel + "/" + cargoFile;
}

bool getInstructionsForCargo(IStowageAlgorithm &algorithm, const std::string &travel, StringToStringVectorMap &map, Port &port) {

    std::optional<std::string> cargoFile = getNextFileForPort(map, port.getId().getCode());  // get cargo file of current port

    if (!cargoFile.has_value()) {  // couldn't find a cargo file
        std::cout << "Warning: no cargo file for current visit, ship will only unload" << std::endl;
        algorithm.getInstructionsForCargo(unloadOnly + port.getId().getCode(), staticOutputFile);  // TODO: find a proper way to communicate the unload
        return true;
    }

    std::string cargoFilePath = getCargoPath(travel, *cargoFile);
    algorithm.getInstructionsForCargo(cargoFilePath, staticOutputFile);

    std::optional<ContainerStorage> containers = readCargoToPortFromFile(cargoFilePath);

    if (!containers.has_value()) {
        std::cout << "Critical warning: couldn't load port information" << std::endl;
        std::cout << "The ship is continuing to the next port..." << std::endl;
        return false;
    }
    port.setStorage(*containers);

    return true;
}

