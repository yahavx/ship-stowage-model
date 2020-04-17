//
// Created by t-yabeny on 4/16/2020.
//

#include "ObjectsReader.h"
#include "FileReader.h"


bool readShipPlan(const std::string &filePath, ShipPlan &shipPlan) {  // TODO: check if indices are 0 or 1 based
    StringStringVector data = readFile(filePath);

    if (!isDataOnlyIntegers(data)) {  // couldn't convert data to int
        return false;
    }

    IntIntVector intData = convertDataToInt(data);

    IntVector firstRow = intData[0];
    if (firstRow.size() < 3) {
        std::cout
                << "Error in loading ship plan from file: insufficient number of arguments for ship dimensions, exiting"
                << std::endl;
        return false;
    }

    int x = firstRow[0], y = firstRow[1], z = firstRow[2];
    std::tuple<int, int, int> dimensions(x, y, z);
    shipPlan.setDimensions(dimensions);

    IntIntVector heights(x, IntVector(y, 0));  // init matrix of size (x,y) with zeroes

    for (IntVector row : intData) {  // iterate on rows
        if (row.size() < 3) {
            std::cout << "Warning in loading ship plan from file: input contains less than 3 arguments, ignoring"
                      << std::endl;
            continue;
        }

        int n = row[0];
        int m = row[1];
        int availableContainers = row[2];

        if (n <= 0 || n >= x || m <= 0 || m >= y) {
            std::cout << "Warning in loading ship plan from file: input exceeds the ship dimensions, ignoring"
                      << std::endl;
            continue;
        }

        if (availableContainers >= z) {
            std::cout
                    << "Warning in loading ship plan from file: input exceeds the maximum available containers, ignoring"
                    << std::endl;
            continue;
        }

        heights[n][m] = z - availableContainers;  // we save the minimum available floor (z is the top)
    }

    shipPlan.setHeights(heights);

    return true;
}


bool readShipRoute(const std::string &filePath, ShipRoute &shipRoute) {
    StringStringVector data = readFile(filePath);

    std::vector<PortId> ports;

    std::string previousPort;  // to check that the same port doesn't appear twice

    for (StringVector row : data) {
        std::string token = row[0];  // ignore extra tokens in a row

        if (!isEnglishWord(token) || token.length() != 5) {
            std::cout
                    << "Error in loading ship route from file: invalid port format, exiting"
                    << std::endl;
            return false;
        }

        ports.push_back(PortId(token));
    }

    shipRoute.setPorts(ports);
}
