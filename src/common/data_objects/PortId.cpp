//
// Created by Orr on 4/9/2020.
//

#include "PortId.h"
#include "../utils/UtilFunctions.h"

// region Constructors

PortId::PortId(const std::string &code) : code(toUpper(code)) {}

// endregion

// region Getters and setters

const std::string &PortId::getCode() const {
    return code;
}

void PortId::setCode(const std::string &code) {
    PortId::code = toUpper(code);
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

// region Functions

bool PortId::isValid() {
    return code.length() == 5 && isEnglishWord(code);
}

// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const PortId &id) {
    os << id.code;
    return os;
}

// endregion
