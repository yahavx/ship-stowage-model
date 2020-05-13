//
// Created by t-yabeny on 5/6/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ALGORITHMVALIDATION_H
#define SHIP_STOWAGE_MODEL_ALGORITHMVALIDATION_H

#include "../common/actors/ContainerShip.h"


/// This class is responsible to perform all needed checks on operations received from the algorithm.
class AlgorithmValidation {
    ContainerShip &ship;
    Port &currentPort;
    StringVector &badContainerIds;
    StringVector temporaryContainersOnPort;  // ids of containers that should be loaded back
    Errors &errors;

public:

    // region Constructor

    AlgorithmValidation(ContainerShip &ship, Port &currentPort, StringVector &badContainerIds, Errors &errors);

    // endregion

    // region Validations

private:
    bool validatePosition(const Position &pos);

    bool validateLoadOperation(const PackingOperation &op);

    bool validateUnloadOperation(const PackingOperation &op);

    bool validateMoveOperation(const PackingOperation &op);

    bool validateRejectOperation(const PackingOperation &op);

public:
    bool validatePackingOperation(const PackingOperation &op);

    bool validateNoContainersLeftOnPort();

    // endregion

    // region Functions

private:
    /// Check if a container (id) is in badContainerIds.
    bool isBadContainer(const std::string &id);

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_ALGORITHMVALIDATION_H
