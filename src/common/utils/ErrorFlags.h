//
// Created by t-yabeny on 5/1/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ERRORFLAGS_H
#define SHIP_STOWAGE_MODEL_ERRORFLAGS_H


#include "Definitions.h"

enum ErrorFlags : int {
    ShipPlan_InvalidFloorHeight = 1 << 0,
    ShipPlan_InvalidXYCoordinates = 1 << 1,
    ShipPlan_BadLineFormat = 1 << 2,
    ShipPlan_FatalError = 1 << 3,

    ShipRoute_TwoConsecutiveSamePort = 1 << 5,
    ShipRoute_BadPortSymbol = 1 << 6,
    ShipRoute_FatalError = 1 << 7,
    ShipRoute_FatalError_SinglePort = 1 << 8,

    Containers_DuplicateID = 1 << 10,
    Containers_IDAlreadyOnShip = 1 << 11,
    Containers_MissingOrBadWeight = 1 << 12,  // TODO: this is another section (cargo data) - change prefix (Containers to Cargo data or something)
    Containers_MissingOrBadPortDest = 1 << 13,
    Containers_MissingContainerID = 1 << 14,
    Containers_BadContainerID = 1 << 15,
    Containers_InvalidFile = 1 << 16,
    Containers_LastPortHasContainers = 1 << 17,
    Containers_ContainersExceedsShipCapacity = 1 << 18
};


/// Converts a int that represents a error flags vector, to a string vector, which each entry representing one error.
StringVector errorsFlagsToString(int errorFlags);

#endif //SHIP_STOWAGE_MODEL_ERRORFLAGS_H
