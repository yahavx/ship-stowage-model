//
// Created by yahav on 05/04/2020.
//

#include "PackingOperation.h"
#include "../utils/UtilFunctions.h"

// Constructors
PackingOperation::PackingOperation(const std::string &containerId, const std::tuple<int, int, int> &indices,
                                   PackingType type) : containerId(containerId), indices(indices), type(type) {}


// Getters and setters
const std::string &PackingOperation::getContainerId() const {
    return containerId;
}

const std::tuple<int, int, int> &PackingOperation::getIndices() const {
    return indices;
}

PackingType PackingOperation::getType() const {
    return type;
}


// Printers
std::ostream &operator<<(std::ostream &os, const PackingOperation &operation) {
    os << "containerId: " << operation.containerId << " indices: " << operation.indices << " type: " << (int)operation.type;
    return os;
}
