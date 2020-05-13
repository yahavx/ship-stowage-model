//
// Created by Orr on 4/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CONTAINERPOSITION_H
#define SHIP_STOWAGE_MODEL_CONTAINERPOSITION_H

#include "Container.h"
#include <vector>
#include <tuple>
#include "../utils/Definitions.h"
#include "Position.h"

/// Wraps a container with his position the ship
class ContainerPosition {
    Container &container;
    Position position;

public:
    // region Constructor

    ContainerPosition(Container &container, const Position &position);

    // endregion

    // region Getters and setters

    Container &getContainer() const;

    void setContainer(Container &container);

    const Position &getPosition() const;

    void setPosition(const Position &position);

    int x() const;
    int y() const;
    int z() const;

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_CONTAINERPOSITION_H
