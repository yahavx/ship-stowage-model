//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SHIPROUTE_H
#define SHIP_STOWAGE_MODEL_SHIPROUTE_H


#include <vector>
#include <ostream>
#include "../actors/Port.h"

class ShipRoute {
    std::vector<PortId> ports;

public:
    // Constructors
    ShipRoute();

    ShipRoute(const std::vector<PortId> &ports);


    // Getters and setters
    std::vector<PortId> &getPorts();

    void setPorts(std::vector<PortId> &ports);


    // Printer
    friend std::ostream &operator<<(std::ostream &os, const ShipRoute &route);
};


#endif //SHIP_STOWAGE_MODEL_SHIPROUTE_H
