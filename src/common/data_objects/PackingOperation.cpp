//
// Created by yahav on 05/04/2020.
//

#include "PackingOperation.h"
#include "../utils/UtilFunctions.h"
#include "../utils/Printers.h"

// region Constructors

PackingOperation::PackingOperation(PackingType type, const std::string &containerId,
                                   const std::tuple<int, int, int> &fromPosition,
                                   const std::tuple<int, int, int> &toPosition) : type(type), containerId(containerId),
                                                                                  fromPosition(fromPosition),
                                                                                  toPosition(toPosition) {}

PackingOperation::PackingOperation(PackingType type, const std::string &containerId,
                                   const std::tuple<int, int, int> &fromPosition) : type(type),
                                                                                    containerId(containerId),
                                                                                    fromPosition(fromPosition),
                                                                                    toPosition({-1, -1, -1}){}
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

std::ostream &operator<<(std::ostream &os, const PackingOperation &operation) {
    os << "Op('" << packingTypeFromString(operation.type)  << "', ContainerID=" << operation.containerId;
    os << ", Position=" << operation.fromPosition;
    if (std::get<0>(operation.toPosition) != -1) {  // TODO: make adaptions after we add move operation
        os << " To " << operation.toPosition;
    }
    os << ")";
    return os;
}
// endregion