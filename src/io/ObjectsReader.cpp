//
// Created by t-yabeny on 4/16/2020.
//

#include "ObjectsReader.h"
#include "FileReader.h"
#include "../utils/UtilFunctions.h"


bool readShipPlanFromFile(const std::string &filePath, ShipPlan &shipPlan) {  // TODO: check if indices are 0 or 1 based
    std::cout << "Attempting to read ship plan..." << std::endl;
    StringStringVector data = readFile(filePath);

    if (!isDataOnlyIntegers(data)) {  // couldn't convert data to int
        return false;
    }

    IntIntVector intData = convertDataToInt(data);

    IntVector firstRow = intData[0];
    if (firstRow.size() < 3) {
        std::cout
                << "Error: insufficient number of arguments for ship dimensions, exiting"
                << std::endl;
        return false;
    }

    int x = firstRow[0], y = firstRow[1], z = firstRow[2];
    std::tuple<int, int, int> dimensions(x, y, z);
    shipPlan.setDimensions(dimensions);

    IntIntVector heights(x, IntVector(y, 0));  // init matrix of size (x,y) with zeroes

    for (IntVector intDataRow : intData) {  // iterate on rows
        if (intDataRow.size() < 3) {
            std::cout << "Warning: data row contains less than 3 arguments, ignoring"
                      << std::endl;
            continue;
        }

        int n = intDataRow[0];
        int m = intDataRow[1];
        int availableContainers = intDataRow[2];

        if (n <= 0 || n >= x || m <= 0 || m >= y) {
            std::cout << "Warning: data row exceeds the ship dimensions, ignoring"
                      << std::endl;
            continue;
        }

        if (availableContainers >= z) {
            std::cout
                    << "Warning: data row exceeds the maximum available containers, ignoring"
                    << std::endl;
            continue;
        }

        heights[n][m] = z - availableContainers;  // we save the minimum available floor (z is the top)
    }

    shipPlan.setHeights(heights);

    std::cout << "Read ship plan successfully." << std::endl;

    return true;
}


bool readShipRouteFromFile(const std::string &filePath, ShipRoute &shipRoute) {
    std::cout << "Attempting to read ship route..." << std::endl;
    StringStringVector data = readFile(filePath);

    std::vector<PortId> ports;

    std::string previousPort;  // to check that the same port doesn't appear twice

    for (StringVector dataRow : data) {
        std::string token = dataRow[0];  // ignore extra tokens in a row

        if (!isEnglishWord(token) || token.length() != 5) {
            std::cout
                    << "Warning: invalid port format, ignoring"
                    << std::endl;
            continue;
        }

        ports.push_back(PortId(token));
    }

    std::cout << "Read ship route successfully." << std::endl;
    shipRoute.setPorts(ports);

    return true;
}

bool readCargoToPortFromFile(const std::string &filePath, Port &port) {
    std::cout << "Attempting to read cargo data..." << std::endl;
    StringStringVector data = readFile(filePath);

    Containers containersToAdd;

    for (StringVector dataRow : data) {
        if (dataRow.size() < 3) {
            std::cout << "Warning: data row contains less than 3 arguments, ignoring"
                      << std::endl;
            continue;
        }

        std::string id = dataRow[0], weight = dataRow[1], destPort = dataRow[2];

        if (!Port::isIdInIsoFormat(id)) {
            std::cout << "Warning: container id not in ISO format, ignoring"
                      << std::endl;
            continue;
        }

        if (!isInteger(weight)) {
            std::cout << "Warning: container weight is not an integer, ignoring"
                      << std::endl;
            continue;
        }

        if (!isEnglishWord(destPort) || destPort.length() != 5) {
            std::cout
                    << "Warning: port symbol is invalid, ignoring"
                    << std::endl;
            continue;
        }

        containersToAdd.push_back(Container(id, stringToInt(weight), PortId(destPort)));
    }

    std::cout << "Read cargo data successfully." << std::endl;
    port.addContainers(containersToAdd);

    return true;
}
