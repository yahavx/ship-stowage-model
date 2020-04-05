//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CONTAINER_H
#define SHIP_STOWAGE_MODEL_CONTAINER_H

#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;

class Container {
    int weight;
    string destPort;
    string id;

public:
    Container(int weight, const string &destPort, const string &id);

    int getWeight() const;

    const string &getDestPort() const;

    const string &getId() const;

    // TODO: remove setters
    void setWeight(int weight);

    void setDestPort(const string &destPort);

    void setId(const string &id);
};

std::ostream &operator<<(std::ostream &strm, const Container &cont);


#endif //SHIP_STOWAGE_MODEL_CONTAINER_H
