//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CONTAINER_H
#define SHIP_STOWAGE_MODEL_CONTAINER_H

#include <string>
#include <iostream>
#include <stdio.h>

class Container {
    int weight;
    std::string destPort;
    std::string id;

public:
    Container(int weight, const std::string &destPort, const std::string &id);

    int getWeight() const;

    const std::string &getDestPort() const;

    const std::string &getId() const;

    // TODO: remove setters
    void setWeight(int weight);

    void setDestPort(const std::string &destPort);

    void setId(const std::string &id);
};

std::ostream &operator<<(std::ostream &strm, const Container &cont);


#endif //SHIP_STOWAGE_MODEL_CONTAINER_H
