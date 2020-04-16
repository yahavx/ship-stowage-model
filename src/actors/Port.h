//
// Created by yahav on 06/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_PORT_H
#define SHIP_STOWAGE_MODEL_PORT_H

#include <iostream>
#include <stdio.h>
#include "../data_structures/ContainerStorage.h"
#include "../data_objects/PortId.h"

class Port {
    PortId id;
    ContainerStorage storage;

public:
    Port(const PortId &id);

    const PortId &getId() const;

    void setId(const PortId &id);

    std::vector<Container> getContainersForDestination(const PortId &destId) ;

    void addContainer(const Container &container);

    const Container &removeContainer(std::string containerId);

    friend std::ostream &operator<<(std::ostream &os, const Port &port);
};


#endif //SHIP_STOWAGE_MODEL_PORT_H
