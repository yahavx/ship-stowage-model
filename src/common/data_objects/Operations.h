//
// Created by yahav on 14/05/2020.
//

#ifndef SHIP_STOWAGE_MODEL_OPERATIONS_H
#define SHIP_STOWAGE_MODEL_OPERATIONS_H


#include "PackingOperation.h"

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


#endif //SHIP_STOWAGE_MODEL_OPERATIONS_H
