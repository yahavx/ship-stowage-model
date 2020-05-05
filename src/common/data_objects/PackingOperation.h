//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_PACKINGOPERATION_H
#define SHIP_STOWAGE_MODEL_PACKINGOPERATION_H

#include <tuple>
#include <ostream>
#include <vector>
//#include "../utils/Definitions.h"

typedef std::vector<std::string> StringVector;

enum class PackingType {
    load, unload, reject, move
};

/// Instructions for loading and unloading cargo to the ship
class PackingOperation {
    PackingType type;

    std::string containerId;

    std::tuple<int, int, int> pos1;

    std::tuple<int, int, int> pos2;  // in case of a 'move'

public:
    // region Constructors

    PackingOperation(PackingType type, const std::string &containerId);  // for reject

    PackingOperation(PackingType type, const std::string &containerId, const std::tuple<int, int, int> &pos1);

    PackingOperation(PackingType type, const std::string &containerId, const std::tuple<int, int, int> &pos1,
                     const std::tuple<int, int, int> &pos2);
    // endregion

    // region Getters and setters

    const std::string &getContainerId() const;

    const std::tuple<int, int, int> &getFirstPosition() const;

    const std::tuple<int, int, int> &getSecondPosition() const;

    PackingType getType() const;
    // endregion

    // region Printer

    friend std::ostream &operator<<(std::ostream &os, const PackingOperation &operation);

    std::string toString() const;

    // endregion
};

class Operations {

public:
    std::vector<PackingOperation> ops;

    // region Functions

    void addOperation(const PackingOperation &op);

    void addOperations(const Operations& ops);

    /// Adds reject operation for all the ids.
    void addRejectOperations(const StringVector &containerIds);

    int size() const;

    bool empty() const;

    // endregion

    // region Printer

    friend std::ostream &operator<<(std::ostream &os, const Operations &operations);

    // endregion
};

#endif //SHIP_STOWAGE_MODEL_PACKINGOPERATION_H
