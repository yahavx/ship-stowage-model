//
// Created by yahav on 05/04/2020.
//

#include "PackingOperation.h"
#include "../utils/UtilFunctions.h"
#include "../utils/Printers.h"


// region Constructors

PackingOperation::PackingOperation(PackingType type, const std::string &containerId) : type(type), containerId(containerId), pos1({-1, -1, -1}),
                                                                                       pos2({-1, -1, -1}) {}

PackingOperation::PackingOperation(PackingType type, const std::string &containerId, const Position &pos1) : type(type),
                                                                                                                              containerId(containerId),
                                                                                                                              pos1(pos1), pos2({-1, -1, -1}) {}

PackingOperation::PackingOperation(PackingType type, const std::string &containerId, const Position &pos1,
                                   const Position &pos2) : type(type), containerId(containerId), pos1(pos1), pos2(pos2) {}

// endregion

// region Getters and setters

PackingType PackingOperation::getType() const {
    return type;
}

const std::string &PackingOperation::getContainerId() const {
    return containerId;
}

const Position &PackingOperation::getFirstPosition() const {
    return pos1;
}

const Position &PackingOperation::getSecondPosition() const {
    return pos2;
}

// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const PackingOperation &operation) {
    os << operation.toString();
    return os;
}

std::string PackingOperation::operationToString() const {
    switch (type) {
        case PackingType::load:
            return "load";
        case PackingType::unload:
            return "unload";
        case PackingType::reject:
            return "reject";
        case PackingType::move:
            return "move";
    }

    return "ERROR";  // stupid compiler
}

std::string PackingOperation::toString() const {
    int x1 = pos1.X(), y1 = pos1.Y(), z1 = pos1.floor();
    int x2 = pos2.X(), y2 = pos2.Y(), z2 = pos2.floor();

    std::string p1 = "(" + intToStr(x1) + ", " + intToStr(y1) + "), floor " + intToStr(z1);
    std::string p2 = "(" + intToStr(x2) + ", " + intToStr(y2) + "), floor " + intToStr(z2);

    switch (type) {
        case PackingType::load:
            return "Load container '" + containerId + "' to position " + p1;
        case PackingType::unload:
            return "Unload container '" + containerId + "' from position " + p1;
        case PackingType::reject:
            return "Reject container '" + containerId + "'";
        case PackingType::move:
            return "Move container '" + containerId + "' from " + p1 + " to " + p2;
    }

    return "Invalid operation";
}

// endregion

