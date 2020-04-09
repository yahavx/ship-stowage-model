//
// Created by Orr on 4/9/2020.
//

#include "PortId.h"
const std::string &PortId::getCode() const {
    return code;
}

void PortId::setCode(const std::string &code) {
    PortId::code = code;
}

PortId::PortId(const std::string &code) : code(code) {}

std::ostream &operator<<(std::ostream &os, const PortId &id) {
    os << "code: " << id.code;
    return os;
}