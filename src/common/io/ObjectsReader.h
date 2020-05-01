//
// Created by t-yabeny on 4/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_OBJECTSREADER_H
#define SHIP_STOWAGE_MODEL_OBJECTSREADER_H

#include "../data_objects/ShipPlan.h"
#include "../data_objects/ShipRoute.h"


/**
 * Reads a ship plan from a file.
 * @param errorsFlag will be set with all the errors that are found in the file
 */
ShipPlan readShipPlanFromFile(const std::string &filePath, int &errorsFlag);

/// Reads a route file (list of ports).
ShipRoute readShipRouteFromFile(const std::string &filePath, int &errorsFlag);

/// Returns a port with the cargo loaded from file. Port id is as in the file (assuming path is ID_<num>.cargo_data).
ContainerStorage readPortCargoFromFile(const std::string &filePath);

/// Reads operations from a file.
std::optional<OPS> readPackingOperationsFromFile(const std::string &filePath);

/// Write operations to a file.
bool writePackingOperationsToFile(const std::string &filePath, OPS &operations);


#endif //SHIP_STOWAGE_MODEL_OBJECTSREADER_H
