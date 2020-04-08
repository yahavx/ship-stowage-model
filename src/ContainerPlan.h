//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CONTAINERPLAN_H
#define SHIP_STOWAGE_MODEL_CONTAINERPLAN_H


#include <vector>
#include "Container.h"
#include "ContainerShip.h"

class ContainerShip;

class FloorPlan {
    std::vector<std::vector<Container>> floorContainers;
};

class ShipPlan {
    std::vector<FloorPlan> shipPlan;  // entry i is the floor plan of floor i

    std::vector<std::vector<int>> heights;  // entry (x,y) is the minimum height allowed to place containers in position (x,y) in the ship

public:
    int getMinimumHeight(ContainerShip &containerShip, int x, int y);  // get the minimum height of (x,y)
};



#endif //SHIP_STOWAGE_MODEL_CONTAINERPLAN_H
