//
// Created by t-yabeny on 5/2/2020.
//

#include "BadAlgorithm.h"
#include "../common/io/ObjectsReader.h"
#include "../common/utils/UtilFunctions.h"

Operations BadAlgorithm::generateOperations(ContainerShip &ship, Port &port, const Containers &containersToLoad) {
    return Operations();
    (void) ship;
    (void) port;
    (void) containersToLoad;
}

int BadAlgorithm::readShipPlan(const std::string &shipPlanPath) {
    return SemiAbstractAlgorithm::readShipPlan(shipPlanPath);
    return 1 << 2;
}
