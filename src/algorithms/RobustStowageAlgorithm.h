//
// Created by Orr on 5/6/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ROBUSTSTOWAGEALGORITHM_H
#define SHIP_STOWAGE_MODEL_ROBUSTSTOWAGEALGORITHM_H

#include "SemiAbstractAlgorithm.h"


class RobustStowageAlgorithm : public SemiAbstractAlgorithm {

private:

    Operations generateOperations(ContainerShip &ship, Port &port, const Containers &containersToLoad, Errors &errors) override ;

    Containers getContainersToLoad(Port &port) override;

    Operations unloadContainer(ContainerShip &ship, Port &port, const ContainerPosition &containerPos,
                               std::vector<ContainerPosition> containersToUnload, std::vector<ContainerPosition> unloadedContainers, Containers &ContainersToLoadBack);

    /// Moves container to a free position if possible
    std::optional<Operations> tryMovingContainer(ContainerShip &ship, Port &port, ContainerPosition &pos, std::vector<ContainerPosition> &containersToUnload);

    /**
     *  Searches for a free position that non of the container in this x,y is meant to be unloaded.
     */

    std::optional<Position> findPosToMoveTo(ContainerShip &ship, std::vector<ContainerPosition> &containersToUnload);
};


#endif //SHIP_STOWAGE_MODEL_ROBUSTSTOWAGEALGORITHM_H
