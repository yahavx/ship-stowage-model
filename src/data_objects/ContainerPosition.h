//
// Created by Orr on 4/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CONTAINERPOSITION_H
#define SHIP_STOWAGE_MODEL_CONTAINERPOSITION_H

#include "Container.h"
#include <vector>
#include <tuple>
#include "../utils/Definitions.h"

/// Wraps a container with his position the ship
class ContainerPosition {
    Container &container;
    POS position;

public:
    // Constructors
    ContainerPosition(Container &container, const POS &position);


    // Getters and setters
    Container &getContainer() const;

    void setContainer(Container &container);

    const POS &getPosition() const;

    void setPosition(const POS &position);

    int x() const;
    int y() const;
    int z() const;
};


#endif //SHIP_STOWAGE_MODEL_CONTAINERPOSITION_H
