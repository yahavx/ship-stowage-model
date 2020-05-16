//
// Created by Orr on 5/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_LOADCONTAINERTOARBITRARYPOSITION_H
#define SHIP_STOWAGE_MODEL_LOADCONTAINERTOARBITRARYPOSITION_H


#include "LoadContainerStrategy.h"
#include "../actors/ContainerShip.h"
#include "../actors/CranesManagement.h"

class LoadContainerToArbitraryPosition : public LoadContainerStrategy {

public:
    Operations loadContainer(Port &port, ContainerShip &ship, const Container &container, Errors &errors) override;

private:
    std::optional<Operations> findFreeSpotToLoadTo(Port &port, ContainerShip &ship, const Container &container);

    std::optional<Operations> createOperations(const Container &container, CranesManagement &crane, Position pos);
};


#endif //SHIP_STOWAGE_MODEL_LOADCONTAINERTOARBITRARYPOSITION_H
