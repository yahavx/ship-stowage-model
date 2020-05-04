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

bool PortId::isIdInIsoFormat() {
    if (code.length() != 11){
        return false;
    }

    if (!isEnglishWord(code.substr(0, 4)) || !isInteger(code.substr(4, 7))){
        return false;
    }

    return true;
}

// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const PortId &id) {
    os << "PortCode: " << id.code;
    return os;
}

// endregion
