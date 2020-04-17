//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SHIPPLAN_H
#define SHIP_STOWAGE_MODEL_SHIPPLAN_H


#include <vector>
#include <tuple>
#include <ostream>
#include "Container.h"

class ShipPlan {
    std::tuple<int,int,int> dimensions;

    std::vector<std::vector<int>> heights;

public:
    // Constructors
    ShipPlan();

    ShipPlan(const std::tuple<int, int, int> &dimensions, const std::vector<std::vector<int>> &heights);


    // Functions
    int getMinimumHeight(int x, int y);  // get the minimum height of (x,y)


    // Getters and setters
    const std::tuple<int, int, int> &getDimensions() const;

    void setDimensions(const std::tuple<int, int, int> &dimensions);

    const std::vector<std::vector<int>> &getHeights() const;

    void setHeights(const std::vector<std::vector<int>> &heights);


    // Printer
    friend std::ostream &operator<<(std::ostream &os, const ShipPlan &plan);
};


#endif //SHIP_STOWAGE_MODEL_SHIPPLAN_H
