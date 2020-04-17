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
    std::tuple<int,int,int> position;

public:
    ContainerPosition(Container &container, const std::tuple<int, int, int> &position);

    Container &getContainer() const;

    void setContainer(Container &container);

    const std::tuple<int, int, int> &getPosition() const;

    void setPosition(const std::tuple<int, int, int> &position);


};


#endif //SHIP_STOWAGE_MODEL_CONTAINERPOSITION_H
