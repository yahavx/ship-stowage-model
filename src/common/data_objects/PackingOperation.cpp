//
// Created by yahav on 05/04/2020.
//

#include "PackingOperation.h"
#include "../utils/UtilFunctions.h"
#include "../utils/Printers.h"

// region class PackingOperation

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

int PackingOperation::getFirstPositionX() const {
    return std::get<0>(pos1);
}

int PackingOperation::getFirstPositionY() const {
    return std::get<1>(pos1);
}

int PackingOperation::getFirstPositionFloor() const {
    return std::get<2>(pos1);
}

const std::tuple<int, int, int> &PackingOperation::getSecondPosition() const {
    return pos2;
}

int PackingOperation::getSecondPositionX() const {
    return std::get<0>(pos2);
}

int PackingOperation::getSecondPositionY() const {
    return std::get<1>(pos2);
}

int PackingOperation::getSecondPositionFloor() const {
    return std::get<2>(pos2);
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
    int x1 = std::get<0>(pos1), y1 = std::get<1>(pos1), z1 = std::get<2>(pos1);
    int x2 = std::get<0>(pos2), y2 = std::get<1>(pos2), z2 = std::get<2>(pos2);
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

// endregion

// region class Operations

// region Functions

void Operations::addOperation(const PackingOperation &op) {
    ops.push_back(op);
}

void Operations::addOperations(const Operations &otherOps) {
    ops.insert(ops.end(), otherOps.ops.begin(), otherOps.ops.end());
}

void Operations::addRejectOperations(const StringVector &containerIds) {
    for (auto& contId: containerIds) {
          ops.push_back({PackingType::reject, contId});
    }
}

int Operations::size(bool excludeRejects) const {
    if (!excludeRejects)
        return ops.size();

    int count = 0;

    for (auto& op : ops) {
        if (op.getType() != PackingType::reject) {
            count++;
        }
    }

    return count;
}

bool Operations::empty() const {
    return ops.empty();
}

// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const Operations &ops) {

        if (ops.size() == 0) {
            std::cout << "<NoOperations>" << std::endl;
            return os;
        }
        std::cout << "Operations {" << std::endl;
        for (int i = 0; i < ops.size(); i++) {
            std::cout << '\t' << ops.ops[i].toString() << std::endl;
        }
        std::cout << "}" << std::endl;
        return os;
}

// endregion

// endregion

