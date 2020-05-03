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
    // region Constructors

    explicit PortId(const std::string &code);

    // endregion

    // region Getters and setters

    const std::string &getCode() const;

    void setCode(const std::string &code);

    // endregion

    // region Operators

    bool operator==(const PortId &rhs) const;

    bool operator!=(const PortId &rhs) const;

    operator std::string() const;

    // endregion

    // region Printer

    friend std::ostream &operator<<(std::ostream &os, const PortId &id);

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_PORTID_H
