//
// Created by yahav on 05/04/2020.
//

#include "ShipPlan.h"
#include "../utils/Printers.h"


// region Constructors

ShipPlan::ShipPlan() {

}

// endregion

ShipPlan::ShipPlan(const Dimensions &dimensions, const std::vector<std::vector<int>> &heights)
        : dimensions(dimensions), heights(heights) {}


// region Functions

int ShipPlan::getMinimumHeight(int x, int y) {
    return 0;
    std::cout << x << y;  // TODO: remove
}

// endregion

// region Getters and setters

const Dimensions &ShipPlan::getDimensions() const {
    return dimensions;
}

void ShipPlan::setDimensions(const Dimensions &dimensions) {
    ShipPlan::dimensions = dimensions;
}

const std::vector<std::vector<int>> &ShipPlan::getHeights() const {
    return heights;
}

void ShipPlan::setHeights(const std::vector<std::vector<int>> &heights) {
    ShipPlan::heights = heights;
}

// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const ShipPlan &plan) {
    auto dimensions = plan.getDimensions();
    auto heights = plan.getHeights();

    os << "ShipPlan {" << std::endl;
    os << "\tDimensions: " << dimensions << std::endl;
    os << "\tHeights:" << std::endl << heights;
    os << "}" << std::endl;
    return os;
}

// endregion