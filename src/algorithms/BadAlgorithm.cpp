//
// Created by t-yabeny on 5/2/2020.
//

#include "BadAlgorithm.h"
#include "../common/io/ObjectsReader.h"
#include "../common/utils/UtilFunctions.h"

// region Initialization

std::string BadAlgorithm::getAlgorithmName() {
    return "BadAlgorithm";
}
// endregion

Operations BadAlgorithm::generateOperations(ContainerShip &ship, Port &port, const Containers &containersToLoad) {
    return Operations();
    (void) ship;
    (void) port;
    (void) containersToLoad;
}
