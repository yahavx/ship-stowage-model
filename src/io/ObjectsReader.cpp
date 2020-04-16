//
// Created by t-yabeny on 4/16/2020.
//

#include "ObjectsReader.h"
#include "InputOutput.h"



bool readShipPlan(const std::string &filePath, ShipPlan &shipPlan) {  // TODO: check if indices are 0 or 1 based
    StringStringVector data = readFile(filePath);

    if (!isDataOnlyIntegers(data)) {  // couldn't convert data to int
        return false;
    }

    IntIntVector intData = convertDataToInt(data);

    int x = intData[0][0], y = intData[0][1], z = intData[0][2];
    std::tuple<int, int, int> dimensions(x, y, z);
    shipPlan.setDimensions(dimensions);

    IntIntVector heights(x, IntVector(y, 0));  // init matrix of size (x,y) with zeroes

    for (int i = 1; i < intData.size(); i++) {  // iterate on rows
        int n = intData[i][0];
        int m = intData[i][1];
        int availableContainers = intData[i][2];

        if (n <= 0 || n >= x || m <= 0 || m >= y) {
            std::cout << "Warning: given line exceeds the ship dimensions, ignoring";
            continue;
        }

        if (availableContainers >= z) {
            std::cout << "Warning: given line exceeds the maximum available containers, ignoring";
            continue;
        }

        heights[n][m] = z - availableContainers + 1;  // we save the minimum available floor (z is the top)
    }

    shipPlan.setHeights(heights);

    return true;
}

