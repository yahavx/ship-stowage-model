//
// Created by t-yabeny on 5/2/2020.
//

#include "BadAlgorithm.h"
#include "../common/io/ObjectsReader.h"
#include "../common/utils/UtilFunctions.h"

Operations BadAlgorithm::generateOperations(ContainerShip &ship, Port &port, const Containers &containersToLoad, Errors &errors) {
    return Operations();
    _unused(ship);
    _unused(port);
    _unused(containersToLoad);
    _unused(errors);
}

int BadAlgorithm::readShipPlan(const std::string &shipPlanPath) {
    return SemiAbstractAlgorithm::readShipPlan(shipPlanPath);
    return 1 << 2;
}
