//
// Created by Orr on 4/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CARGO_H
#define SHIP_STOWAGE_MODEL_CARGO_H

#include "../data_objects/ShipPlan.h"
#include "../data_objects/ContainerPosition.h"

class Cargo {
    std::vector<std::vector<std::vector<Container &>>> containers;
    ShipPlan &shipPlan;

public:
    /**
     * If possible to load the container on top in this x,y position returns the height it would be loaded to.
     * else returns negative integer
     */
    int canLoadContainerOnTop(int x, int y, const Container &container) const;

    int currentHeight(int x, int y) const;


    const Container &getTopContainer(int x, int y);

    const Container &removeTopContainer(int x, int y);


    int loadContainerOnTop(int x, int y, const Container &container);

    std::vector<ContainerPosition> getContainersForPort(const PortId &portId) const;
};


#endif //SHIP_STOWAGE_MODEL_CARGO_H
