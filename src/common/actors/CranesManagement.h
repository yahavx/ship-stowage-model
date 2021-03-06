//
// Created by Orr on 4/17/2020.
//

#ifndef SHIP_STOWAGE_MODEL_CRANESMANAGEMENT_H
#define SHIP_STOWAGE_MODEL_CRANESMANAGEMENT_H


#include "../data_objects/PackingOperation.h"
#include "Port.h"
#include "ContainerShip.h"

enum class CraneOperationResult {
    SUCCESS, FAIL_ILLEGAL_OP, FAIL_CONTAINER_NOT_FOUND
};

class CranesManagement {
    ContainerShip &ship;
    Port &currentPort;

public:

    // region Constructors

    CranesManagement(ContainerShip &ship, Port &currentPort);

    // endregion

    // region Functions

    /**
     * Perform packing operation if is legal operation.
     * @param op - the operation to perform
     * @param port - the port to load/unload from/to
     * @param ship - the ship to load/unload to/from
     * @return operation result status
     */
    CraneOperationResult preformOperation(const PackingOperation &op);

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_CRANESMANAGEMENT_H
