//
// Created by Orr on 4/9/2020.
//

#include "PortId.h"

// Constructors
PortId::PortId(const std::string &code) : code(code) {}


// Getters and setters
const std::string &PortId::getCode() const {
    return code;
}

void PortId::setCode(const std::string &code) {
    PortId::code = code;
}


// Printer
std::ostream &operator<<(std::ostream &os, const PortId &id) {
    os << "PortCode: " << id.code;
    return os;
}

bool PortId::operator==(const PortId &rhs) const {
    return code == rhs.code;
}

bool PortId::operator!=(const PortId &rhs) const {
    return !(rhs == *this);
}
