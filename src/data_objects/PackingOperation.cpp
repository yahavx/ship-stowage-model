//
// Created by yahav on 05/04/2020.
//

#include "PackingOperation.h"
#include "../utils/UtilFunctions.h"

// Constructors
PackingOperation::PackingOperation(const std::string &containerId, const std::tuple<int, int, int> &fromPosition,
                                   PackingType type) : containerId(containerId), fromPosition(fromPosition),
                                                       type(type) {}  // Only one position

PackingOperation::PackingOperation(const std::string &containerId, const std::tuple<int, int, int> &fromPosition,
                                   const std::tuple<int, int, int> &toPosition, PackingType type) : containerId(
        containerId), fromPosition(fromPosition), toPosition(toPosition), type(type) {}   // two positions (move)


// Getters and setters
const std::string &PackingOperation::getContainerId() const {
    return containerId;
}

PackingType PackingOperation::getType() const {
    return type;
}

const std::tuple<int, int, int> &PackingOperation::getFromPosition() const {
    return fromPosition;
}

const std::tuple<int, int, int> &PackingOperation::getToPosition() const {
    return toPosition;
}


// Printers
std::ostream &operator<<(std::ostream &os, const PackingOperation &operation) {  // TODO
//    os << "containerId: " << operation.containerId << " indices: " << operation.indices << " type: " << (int)operation.type;
    return os;
}

