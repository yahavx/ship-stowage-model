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
    ShipRoute(const std::vector<PortId> &ports);


    // Getters and setters
    const std::vector<PortId> &getPorts() const;

    void setPorts(const std::vector<PortId> &ports);


    // Printer
    friend std::ostream &operator<<(std::ostream &os, const ShipRoute &route);
};


#endif //SHIP_STOWAGE_MODEL_SHIPROUTE_H
