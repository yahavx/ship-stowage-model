//
// Created by Orr on 6/11/2020.
//

#ifndef SHIP_STOWAGE_MODEL_LOADCONTAINERINGROUPS_H
#define SHIP_STOWAGE_MODEL_LOADCONTAINERINGROUPS_H


#include "../data_objects/Operations.h"
#include "../data_objects/Container.h"
#include "../actors/ContainerShip.h"

class LoadContainerInGroups : public LoadContainerStrategy {

public:
    Operations loadContainer(Port &port, ContainerShip &ship, const Container &container, Errors &errors) override;

private:
    std::optional<Position> findLowestSpotWithSameDestination(ContainerShip &ship, const Container &container);
};


#endif //SHIP_STOWAGE_MODEL_LOADCONTAINERINGROUPS_H
