//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SHIPPLAN_H
#define SHIP_STOWAGE_MODEL_SHIPPLAN_H


#include <vector>
#include <tuple>
#include "Container.h"

class ShipPlan {
    std::tuple<int,int,int> dimensions;
    std::vector<std::vector<int>> heights;  // entry (x,y) is the minimum height allowed to place containers in position (x,y) in the ship

public:

    ShipPlan(const std::tuple<int, int, int> &dimensions, const std::vector<std::vector<int>> &heights);

    int getMinimumHeight(int x, int y);  // get the minimum height of (x,y)
};


#endif //SHIP_STOWAGE_MODEL_SHIPPLAN_H
