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
    ShipRoute(const std::vector<PortId> &ports);

    const std::vector<PortId> &getPorts() const;

    friend std::ostream &operator<<(std::ostream &os, const ShipRoute &route);

    void setPorts(const std::vector<PortId> &ports);
};


#endif //SHIP_STOWAGE_MODEL_SHIPROUTE_H
