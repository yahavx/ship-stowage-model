//
// Created by yahav on 05/04/2020.
//

#include "Container.h"
#include "../utils/UtilFunctions.h"

#include <utility>


// region Constructors

Container::Container(const std::string &id, int weight, const PortId &destPort) : id(id), weight(weight),
                                                                                  destPort(destPort) {}
// endregion

// region Getters and setters

int Container::getWeight() const {
    return weight;
}

void Container::setWeight(int weight) {
    Container::weight = weight;
}

const PortId &Container::getDestPort() const {
    return destPort;
}

void Container::setDestPort(const PortId &destPort) {
    Container::destPort = destPort;
}

const std::string &Container::getId() const {
    return id;
}

void Container::setId(const std::string &id) {
    Container::id = id;
}

// endregion

// region Operators

bool Container::operator==(const Container &rhs) const {
    return id == rhs.id;
}

bool Container::operator!=(const Container &rhs) const {
    return !(rhs == *this);
}

// endregion

// region Functions

bool Container::isIdInIsoFormat() const {
    return id.length() == 11
           && !isEnglishWord(id.substr(0, 3))
           && isLegalCategoryIdentifier(id.substr(3, 4))
           && !isInteger(id.substr(4, 11));

}

Error Container::isContainerLegal(bool validateIdFormat = false) const {
    if (validateIdFormat && !isIdInIsoFormat()) {
        return {ErrorFlag::ContainersAtPort_BadContainerID, id};
    }

    if (weight <= 0) {
        return {ErrorFlag::ContainersAtPort_MissingOrBadWeight, intToStr(weight)};
    }

    if (!destPort.isValid()) {
        return {ErrorFlag::ContainersAtPort_MissingOrBadPortDest, destPort};
    }

    return ErrorFlag::Success;
}

// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const Container &container) {
    os << "Container(weight: " << container.weight << ", destPort: " << container.destPort << ", id: " << container.id
       << ")";
    return os;
}

// endregion