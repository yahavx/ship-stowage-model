//
// Created by Orr on 4/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CARGO_H
#define SHIP_STOWAGE_MODEL_CARGO_H

#include "../data_objects/ShipPlan.h"
#include "../data_objects/ContainerPosition.h"


/// Cargo is the stock of containers in the ship
class Cargo {
    std::vector<std::vector<std::vector<Container &>>> containers;

    ShipPlan &shipPlan;

public:
    const Container &getTopContainer(int x, int y);

    const Container &removeTopContainer(int x, int y);

    int addContainerOnTop(int x, int y, const Container &container);

    std::vector<ContainerPosition> getContainersForPort(const PortId &portId) const;
};


#endif //SHIP_STOWAGE_MODEL_CARGO_H
