//
// Created by Orr on 4/9/2020.
//

#include "PortId.h"

// region Constructors

PortId::PortId(const std::string &code) : code(code) {}

// endregion

// region Getters and setters

const std::string &PortId::getCode() const {
    return code;
}

void PortId::setCode(const std::string &code) {
    PortId::code = code;
}

// endregion

// region Operators

bool PortId::operator==(const PortId &rhs) const {
    return code == rhs.code;
}

bool PortId::operator!=(const PortId &rhs) const {
    return !(rhs == *this);
}

PortId::operator std::string() const {
    return code;
}

// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const PortId &id) {
    os << "PortCode: " << id.code;
    return os;
}

// endregion
