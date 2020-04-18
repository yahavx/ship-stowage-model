//
// Created by yahav on 06/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_PORT_H
#define SHIP_STOWAGE_MODEL_PORT_H

#include <iostream>
#include <stdio.h>
#include "../data_structures/ContainerStorage.h"
#include "../data_objects/PortId.h"
#include "../utils/Definitions.h"

class Port {
    PortId id;
    ContainerStorage storage;

public:
    // Constructors
    Port(const PortId &id);

    Port(const PortId &id, const ContainerStorage &storage);


    // Functions
    Containers getContainersForDestination(const PortId &destId);

    void addContainer(const Container &container);

    void addContainers(const Containers &containers);

    const Container &removeContainer(std::string containerId);

    static bool isIdInIsoFormat(const std::string &id);


    // Getters and setters
    const PortId &getId() const;

    void setId(const PortId &id);


    // Printer
    friend std::ostream &operator<<(std::ostream &os, const Port &port);
};


#endif //SHIP_STOWAGE_MODEL_PORT_H
