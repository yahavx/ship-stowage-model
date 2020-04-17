//
// Created by Orr on 4/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CONTAINERPOSITION_H
#define SHIP_STOWAGE_MODEL_CONTAINERPOSITION_H

#include "Container.h"
#include <vector>
#include <tuple>

class ContainerPosition {

private:
    Container &container;
    POS position;

public:
    ContainerPosition(Container &container, const POS &position);

    Container &getContainer() const;

    void setContainer(Container &container);

    const POS &getPosition() const;

    void setPosition(const POS &position);

    int x() const;
    int y() const;
    int z() const;


};


#endif //SHIP_STOWAGE_MODEL_CONTAINERPOSITION_H
