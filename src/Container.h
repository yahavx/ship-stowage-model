//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CONTAINER_H
#define SHIP_STOWAGE_MODEL_CONTAINER_H

#include <string>
#include <iostream>
#include <stdio.h>
#include "Port.h"

class Container {
    int weight;
    Port destPort;
    std::string id;

public:
    Container(int weight, const Port &destPort, std::string id);

    int getWeight() const;

    const Port &getDestPort() const;

    const std::string &getId() const;

    // TODO: remove setters
    void setWeight(int weight);

    void setDestPort(const Port &destPort);

    void setId(const std::string &id);
};

std::ostream &operator<<(std::ostream &strm, const Container &cont);


#endif //SHIP_STOWAGE_MODEL_CONTAINER_H
