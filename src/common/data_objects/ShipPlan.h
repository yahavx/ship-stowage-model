//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SHIPPLAN_H
#define SHIP_STOWAGE_MODEL_SHIPPLAN_H

#include <vector>
#include <tuple>
#include <ostream>
#include "Container.h"
#include "../utils/Definitions.h"
#include "Position.h"
#include "Dimensions.h"


class ShipPlan {
    Dimensions dimensions;
    std::vector<std::vector<int>> heights;  // entry (x,y) is the minimum height allowed to place containers in position (x,y) in the ship

public:
    // region Constructors

    ShipPlan();

    ShipPlan(const Dimensions &dimensions, const std::vector<std::vector<int>> &heights);

    // endregion

    // region Functions

    int getMinimumHeight(int x, int y);  // get the minimum height of (x,y)

    // endregion

    // region Getters and setters

    const Dimensions &getDimensions() const;

    void setDimensions(const Dimensions &dimensions);

    const std::vector<std::vector<int>> &getHeights() const;

    void setHeights(const std::vector<std::vector<int>> &heights);

    // endregion

    // region Printer

    friend std::ostream &operator<<(std::ostream &os, const ShipPlan &plan);

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_SHIPPLAN_H
