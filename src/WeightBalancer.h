//
// Created by yahav on 06/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_WEIGHTBALANCER_H
#define SHIP_STOWAGE_MODEL_WEIGHTBALANCER_H

#include "ContainerShip.h"

class ContainerShip;  // pre declare

class WeightBalancer {
    bool isBalancedAfterLoad(ContainerShip &containerShip, int x, int y, int z);

    bool isBalancedAfterUnload(int x, int y, int z);
};


#endif //SHIP_STOWAGE_MODEL_WEIGHTBALANCER_H
