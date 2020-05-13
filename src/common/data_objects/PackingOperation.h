//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_PACKINGOPERATION_H
#define SHIP_STOWAGE_MODEL_PACKINGOPERATION_H

#include <tuple>
#include <ostream>
#include <vector>
#include "Position.h"
//#include "../utils/Definitions.h"

typedef std::vector<std::string> StringVector;

enum class PackingType {
    load, unload, reject, move
};

/// Instructions for loading and unloading cargo to the ship
class PackingOperation {
    PackingType type;

    std::string containerId;

    Position pos1;

    Position pos2;  // in case of a 'move'

public:
    // region Constructors

    PackingOperation(PackingType type, const std::string &containerId);  // for reject

    PackingOperation(PackingType type, const std::string &containerId, const Position &pos1);

    PackingOperation(PackingType type, const std::string &containerId, const Position &pos1, const Position &pos2);
    // endregion

    // region Getters and setters

    const std::string &getContainerId() const;

    const Position &getFirstPosition() const;

    int getFirstPositionX() const;

    int getFirstPositionY() const;

    int getFirstPositionFloor() const;

    const Position &getSecondPosition() const;

    int getSecondPositionX() const;

    int getSecondPositionY() const;

    int getSecondPositionFloor() const;

    PackingType getType() const;
    // endregion

    // region Printer

    friend std::ostream &operator<<(std::ostream &os, const PackingOperation &operation);

    std::string operationToString() const;

    std::string toString() const;

    // endregion
};

/// A list of packing operations.
class Operations {

public:
    std::vector<PackingOperation> ops;

    // region Functions

    void addOperation(const PackingOperation &op);

    void addOperations(const Operations& ops);

    /// Adds reject operation for all the ids.
    void addRejectOperations(const StringVector &containerIds);

    int size(bool excludeRejects = false) const;

    bool empty() const;

    // endregion

    // region Printer

    friend std::ostream &operator<<(std::ostream &os, const Operations &operations);

    // endregion
};

#endif //SHIP_STOWAGE_MODEL_PACKINGOPERATION_H
