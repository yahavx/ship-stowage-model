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

typedef std::tuple<int, int, int> POS;
typedef  std::vector<PackingOperation> OPS;

class Container {
    int weight;
    PortId destPort;
    std::string id;

public:
    Container(int weight, const PortId &destPort, const std::string &id);

    int getWeight() const;

    void setWeight(int weight);

    const PortId &getDestPort() const;

    void setDestPort(const PortId &destPort);

    const std::string &getId() const;

    void setId(const std::string &id);

    friend std::ostream &operator<<(std::ostream &os, const Container &container);
};


#endif //SHIP_STOWAGE_MODEL_CONTAINER_H
