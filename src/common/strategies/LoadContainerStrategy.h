//
// Created by Orr on 5/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_LOADCONTAINERSTRATEGY_H
#define SHIP_STOWAGE_MODEL_LOADCONTAINERSTRATEGY_H


#include "../actors/ContainerShip.h"

class ContainerShip;

class LoadContainerStrategy {

public:
    virtual Operations loadContainer(Port &port, ContainerShip &ship, const Container &container, Errors &errors) = 0;
};


#endif //SHIP_STOWAGE_MODEL_LOADCONTAINERSTRATEGY_H
