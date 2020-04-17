//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CONTAINER_H
#define SHIP_STOWAGE_MODEL_CONTAINER_H

#include <string>
#include <iostream>
#include <stdio.h>
#include "PortId.h"

class Container {
    std::string id;
    int weight;
    PortId destPort;

public:
    // Constructs
    Container(const std::string &id, int weight, const PortId &destPort);


    // Getters and setters
    int getWeight() const;

    void setWeight(int weight);

    const PortId &getDestPort() const;

    void setDestPort(const PortId &destPort);

    const std::string &getId() const;

    void setId(const std::string &id);


    // Printer
    friend std::ostream &operator<<(std::ostream &os, const Container &container);
};


#endif //SHIP_STOWAGE_MODEL_CONTAINER_H
