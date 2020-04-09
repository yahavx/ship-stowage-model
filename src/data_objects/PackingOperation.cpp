//
// Created by yahav on 05/04/2020.
//

#include "PackingOperation.h"

const std::string &PackingOperation::getContainerId() const {
    return containerId;
}

const std::tuple<int, int, int> &PackingOperation::getIndices() const {
    return indices;
}

PackingType PackingOperation::getType() const {
    return type;
}

PackingOperation::PackingOperation(const std::string &containerId, const std::tuple<int, int, int> &indices,
                                   PackingType type) : containerId(containerId), indices(indices), type(type) {}

std::ostream& operator<<(std::ostream& os, const std::tuple<int,int,int>& tup)
{
    os << "[";
    os << std::get<0>(tup) << ", " << std::get<1>(tup) << ", " << std::get<2>(tup);
    return os << "]";
}


std::ostream &operator<<(std::ostream &os, const PackingOperation &operation) {
    os << "containerId: " << operation.containerId << " indices: " << operation.indices << " type: " << (int)operation.type;
    return os;
}


