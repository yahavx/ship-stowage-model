//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SHIPROUTE_H
#define SHIP_STOWAGE_MODEL_SHIPROUTE_H


#include <vector>
#include <ostream>
#include <unordered_set>
#include "../actors/Port.h"

class ShipRoute {
    std::vector<PortId> ports;

public:
    // region Constructors

    ShipRoute();

    explicit ShipRoute(const std::vector<PortId> &ports);

    // endregion

    // region Getters and setters

    std::vector<PortId> &getPorts();

    void setPorts(std::vector<PortId> &ports);

    const PortId& getFirstPort() const;

    // endregion

    // region Functions

    /// Returns the list of ports ids as a set of strings (without duplicates), excluding the first one.
    std::unordered_set<std::string> getNextPortsSet() const;

    bool isLastPort();

    // endregion

    // region Printer

    friend std::ostream &operator<<(std::ostream &os, const ShipRoute &route);

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_SHIPROUTE_H
