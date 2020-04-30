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


class ShipPlan {
    POS dimensions;
    std::vector<std::vector<int>> heights;  // entry (x,y) is the minimum height allowed to place containers in position (x,y) in the ship

public:
    // Constructors
    ShipPlan();

    ShipPlan(const POS &dimensions, const std::vector<std::vector<int>> &heights);


    // Functions
    int getMinimumHeight(int x, int y);  // get the minimum height of (x,y)


    // Getters and setters
    const POS &getDimensions() const;

    void setDimensions(const POS &dimensions);

    const std::vector<std::vector<int>> &getHeights() const;

    void setHeights(const std::vector<std::vector<int>> &heights);


    // Printer
    friend std::ostream &operator<<(std::ostream &os, const ShipPlan &plan);
};


#endif //SHIP_STOWAGE_MODEL_SHIPPLAN_H
