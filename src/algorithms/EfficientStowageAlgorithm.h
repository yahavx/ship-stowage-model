//
// Created by Orr on 6/10/2020.
//

#ifndef SHIP_STOWAGE_MODEL_EFFICIENTSTOWAGEALGORITHM_H
#define SHIP_STOWAGE_MODEL_EFFICIENTSTOWAGEALGORITHM_H


#include "SemiAbstractAlgorithm.h"

class EfficientStowageAlgorithm : public SemiAbstractAlgorithm {

private:

    Operations generateOperations(ContainerShip &ship, Port &port, const Containers &containersToLoad, Errors &errors) override;

    Containers getContainersToLoad(Port &port) override;

    Operations unloadContainer(ContainerShip &ship, Port &port, const ContainerPosition &containerPos,
            std::vector<ContainerPosition> containersToUnload, std::vector<ContainerPosition> unloadedContainers, Containers &ContainersToLoadBack);

    /// Moves container to a free position if possible
    std::optional<Operations> tryMovingContainer(ContainerShip &ship, Port &port, ContainerPosition &pos, std::vector<ContainerPosition> &containersToUnload);

    /**
     *  Searches for a free position that non of the container in this x,y is meant to be unloaded.
     *  Prefers position which it's top container has the same destination as the given container
     */

    std::optional<Position> findPosToMoveTo(ContainerShip &ship, ContainerPosition &pos, std::vector<ContainerPosition> &containersToUnload);
};


#endif //SHIP_STOWAGE_MODEL_EFFICIENTSTOWAGEALGORITHM_H
