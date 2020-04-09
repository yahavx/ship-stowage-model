//
// Created by yahav on 05/04/2020.
//

#include "ShipPlan.h"

int ShipPlan::getMinimumHeight(int x, int y) {
    return 0;
}

ShipPlan::ShipPlan(const std::tuple<int, int, int> &dimensions, const std::vector<std::vector<int>> &heights)
        : dimensions(dimensions), heights(heights) {}
