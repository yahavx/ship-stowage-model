//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CONTAINER_H
#define SHIP_STOWAGE_MODEL_CONTAINER_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "PortId.h"
#include "PackingOperation.h"


class   Container {
    std::string id;
    int weight;
    PortId destPort;

public:
    // region Constructors

    Container(const std::string &id, int weight, const PortId &destPort);
    // endregion

    // region Getters and setters

    int getWeight() const;


    void setWeight(int weight);

    const PortId &getDestPort() const;

    void setDestPort(const PortId &destPort);

    const std::string &getId() const;

    void setId(const std::string &id);
    // endregion

    // region Printer

    friend std::ostream &operator<<(std::ostream &os, const Container &container);
    // endregion

    bool operator==(const Container &rhs) const;

    bool operator!=(const Container &rhs) const;
};


#endif //SHIP_STOWAGE_MODEL_CONTAINER_H
