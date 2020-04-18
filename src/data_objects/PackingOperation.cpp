//
// Created by yahav on 05/04/2020.
//

#include "PackingOperation.h"
#include "../utils/UtilFunctions.h"

// region Constructors

PackingOperation::PackingOperation(PackingType type, const std::string &containerId,
                                   const std::tuple<int, int, int> &fromPosition,
                                   const std::tuple<int, int, int> &toPosition) : type(type), containerId(containerId),
                                                                                  fromPosition(fromPosition),
                                                                                  toPosition(toPosition) {}

PackingOperation::PackingOperation(PackingType type, const std::string &containerId,
                                   const std::tuple<int, int, int> &fromPosition) : type(type),
                                                                                    containerId(containerId),
                                                                                    fromPosition(fromPosition) {}
// endregion

// region Getters and setters

PackingType PackingOperation::getType() const {
    return type;
}

const std::string &PackingOperation::getContainerId() const {
    return containerId;
}

const std::tuple<int, int, int> &PackingOperation::getFromPosition() const {
    return fromPosition;
}

const std::tuple<int, int, int> &PackingOperation::getToPosition() const {
    return toPosition;
}
// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const PackingOperation &operation) {  // TODO
//    os << "containerId: " << operation.containerId << " indices: " << operation.indices << " type: " << (int)operation.type;
    return os;
}
// endregion