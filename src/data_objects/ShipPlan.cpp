//
// Created by yahav on 05/04/2020.
//

#include "ShipPlan.h"


// Constructors
ShipPlan::ShipPlan() {

}

ShipPlan::ShipPlan(const std::tuple<int, int, int> &dimensions, const std::vector<std::vector<int>> &heights)
        : dimensions(dimensions), heights(heights) {}


// Functions
int ShipPlan::getMinimumHeight(int x, int y) {
    return 0;
}


// Getters and setters
const std::tuple<int, int, int> &ShipPlan::getDimensions() const {
    return dimensions;
}

void ShipPlan::setDimensions(const std::tuple<int, int, int> &dimensions) {
    ShipPlan::dimensions = dimensions;
}

const std::vector<std::vector<int>> &ShipPlan::getHeights() const {
    return heights;
}

void ShipPlan::setHeights(const std::vector<std::vector<int>> &heights) {
    ShipPlan::heights = heights;
}
