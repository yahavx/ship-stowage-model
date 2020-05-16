//
// Created by Orr on 5/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_LOADCONTAINERTOLOWESTPOSITIONAVAILABLE_H
#define SHIP_STOWAGE_MODEL_LOADCONTAINERTOLOWESTPOSITIONAVAILABLE_H


#include "LoadContainerStrategy.h"
#include "../actors/ContainerShip.h"
#include "../actors/CranesManagement.h"

class LoadContainerToLowestPositionAvailable : public LoadContainerStrategy {

public:
    Operations loadContainer(Port &port, ContainerShip &ship, const Container &container, Errors &errors) override;

private:
    std::optional<Position> findLowestSpot(Port &port, ContainerShip &ship, const Container &container);
};


#endif //SHIP_STOWAGE_MODEL_LOADCONTAINERTOLOWESTPOSITIONAVAILABLE_H
