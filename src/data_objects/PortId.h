//
// Created by Orr on 4/9/2020.
//

#ifndef SHIP_STOWAGE_MODEL_PORTID_H
#define SHIP_STOWAGE_MODEL_PORTID_H

#include <iostream>
#include <stdio.h>


class PortId {
    std::string code;

public:
    // Constructors
    explicit PortId(const std::string &code);


    // Getters and setters
    const std::string &getCode() const;

    void setCode(const std::string &code);


    // Printer
    friend std::ostream &operator<<(std::ostream &os, const PortId &id);
};


#endif //SHIP_STOWAGE_MODEL_PORTID_H
