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
        std::cout << "Error: insufficient number of arguments for ship dimensions, exiting" << std::endl;
        return false;
    }

    int x = firstRow[0], y = firstRow[1], z = firstRow[2];
    std::tuple<int, int, int> dimensions(x, y, z);
    shipPlan.setDimensions(dimensions);

    IntIntVector heights(x, IntVector(y, 0));  // init matrix of size (x,y) with zeroes

    for (int i = 1; i < intData.size(); i++) {  // iterate on rows
        IntVector row = intData[i];

        if (row.size() < 3) {
            std::cout << "Warning: give line contains less than 3 arguments, ignoring" << std::endl;
            continue;
        }

        int n = row[0];
        int m = row[1];
        int availableContainers = row[2];

        if (n <= 0 || n >= x || m <= 0 || m >= y) {
            std::cout << "Warning: given line exceeds the ship dimensions, ignoring" << std::endl;
            continue;
        }

        if (availableContainers >= z) {
            std::cout << "Warning: given line exceeds the maximum available containers, ignoring" << std::endl;
            continue;
        }

        heights[n][m] = z - availableContainers + 1;  // we save the minimum available floor (z is the top)
    }

    shipPlan.setHeights(heights);

    return true;
}


bool readRouteFile(const std::string &filePath, ShipRoute &shipRoute) {
    return false;
}