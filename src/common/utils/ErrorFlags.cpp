//
// Created by t-yabeny on 5/1/2020.
//

#include "ErrorFlags.h"


StringVector errorsFlagsToString(int errorsFlag);


/// Converts a single flag to a string.
std::string flagToString(ErrorFlags flag) {
    switch (flag) {
        case ShipPlan_InvalidFloorHeight:
            return "Ship plan warning: data row exceeds the maximum available containers, ignored";
        case ShipPlan_InvalidXYCoordinates:
            return "Ship plan warning: data row exceeds the ship dimensions, ignored";
        case ShipPlan_BadLineFormat:
            return "Ship plan warning: data row is in invalid format, ignored";
        case ShipPlan_FatalError:
            return "Ship plan fatal error: invalid first line, or couldn't read file, terminating the travel";

        case ShipRoute_TwoConsecutiveSamePort:
            return "Ship route warning: a route appeared twice in a row";
        case ShipRoute_BadPortSymbol:
            return "Ship route warning: invalid port symbol format";
        case ShipRoute_FatalError:
            return "Ship route fatal error: no ports in the route, or couldn't read file, terminating the travel";
        case ShipRoute_FatalError_SinglePort:
            return "Ship route fatal error: only one port appears in the route, terminating the travel";

        case Containers_DuplicateID:
            return "Containers at port warning: duplicate container ID found, rejecting";
        case Containers_IDAlreadyOnShip:
            return "Containers at port warning: container with the same ID already on the ship, rejecting";
        case Containers_MissingOrBadWeight:
            return "Cargo data warning: missing or bad weight, ignoring";
        case Containers_MissingOrBadPortDest:
            return "Cargo data warning: missing or bad destination port, ignoring";
        case Containers_MissingContainerID:
            return "Cargo data warning: missing container ID, ignoring";
        case Containers_BadContainerID:
            return "Cargo data warning: container ID is not in ISO 6346 format, ignoring";
        case Containers_InvalidFile:
            return "Cargo data warning: couldn't read any container from file, cargo will only be loaded";
        case Containers_LastPortHasContainers:
            return "Containers at port warning: last port has waiting containers, ignored";
        case Containers_ContainersExceedsShipCapacity:
            return "Containers at port warning: ship is at full capacity, far containers will not be loaded";`
    }
}
