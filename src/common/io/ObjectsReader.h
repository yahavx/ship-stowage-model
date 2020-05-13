//
// Created by t-yabeny on 4/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_OBJECTSREADER_H
#define SHIP_STOWAGE_MODEL_OBJECTSREADER_H

#include "../data_objects/ShipPlan.h"
#include "../data_objects/ShipRoute.h"
#include "../utils/Errors.h"
#include "../data_objects/Operations.h"


/**
 * Reads a ship plan from a file.
 * @param errors each error found will be added to this
 */
ShipPlan readShipPlanFromFile(const std::string &filePath, Errors &errors);

/// Reads a route file (list of ports).
ShipRoute readShipRouteFromFile(const std::string &filePath, Errors &errors);

/// Returns all cargo found in file.
ContainerStorage readPortCargoFromFile(const std::string &filePath, Errors &errors);

/// Reads operations from a file.
Operations readPackingOperationsFromFile(const std::string &filePath, Errors &errors);

/// Write operations to a file.
bool writePackingOperationsToFile(const std::string &filePath, Operations &operations);


#endif //SHIP_STOWAGE_MODEL_OBJECTSREADER_H
