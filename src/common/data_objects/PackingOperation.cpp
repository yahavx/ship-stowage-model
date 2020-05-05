//
// Created by yahav on 05/04/2020.
//

#include "PackingOperation.h"
#include "../utils/UtilFunctions.h"
#include "../utils/Printers.h"

// region Constructors

PackingOperation::PackingOperation(PackingType type, const std::string &containerId) : type(type), containerId(containerId), pos1({-1, -1, -1}),
                                                                                       pos2({-1, -1, -1}) {}

PackingOperation::PackingOperation(PackingType type, const std::string &containerId, const std::tuple<int, int, int> &pos1) : type(type),
                                                                                                                              containerId(containerId),
                                                                                                                              pos1(pos1), pos2({-1, -1, -1}) {}

PackingOperation::PackingOperation(PackingType type, const std::string &containerId, const std::tuple<int, int, int> &pos1,
                                   const std::tuple<int, int, int> &pos2) : type(type), containerId(containerId), pos1(pos1), pos2(pos2) {}

// endregion

// region Getters and setters

PackingType PackingOperation::getType() const {
    return type;
}

const std::string &PackingOperation::getContainerId() const {
    return containerId;
}

const std::tuple<int, int, int> &PackingOperation::getFirstPosition() const {
    return pos1;
}

const std::tuple<int, int, int> &PackingOperation::getSecondPosition() const {
    return pos2;
}

// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const PackingOperation &operation) {
    os << "Op('" << packingTypeFromString(operation.type) << "', ContainerID=" << operation.containerId;
    os << ", Position=" << operation.pos1;
    if (std::get<0>(operation.pos2) != -1) {  // TODO: make adaptions after we add move operation
        os << " To " << operation.pos2;
    }
    os << ")";
    return os;
}

std::string PackingOperation::toString() const {
    int x1 = std::get<0>(pos1), y1 = std::get<1>(pos1), z1 = std::get<2>(pos1);
    int x2 = std::get<0>(pos2), y2 = std::get<1>(pos2), z2 = std::get<2>(pos2);
    std::string pos1 = "(" + intToStr(x1) + ", " + intToStr(y1) + "), floor " + intToStr(z1);
    std::string pos2 = "(" + intToStr(x2) + ", " + intToStr(y2) + "), floor " + intToStr(z2);

    switch (type) {
        case PackingType::load:
            return "Load container '" + containerId + "' to position " + pos1;
        case PackingType::unload:
            return "Unload container '" + containerId + "' from position " + pos1;
        case PackingType::reject:
            return "Reject container '" + containerId + "'";
        case PackingType::move:
            return "Move container '" + containerId + "' from " + pos1 + " to " + pos2;
    }

    return "Invalid operation";
}

// endregion