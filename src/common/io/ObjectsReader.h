//
// Created by t-yabeny on 4/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_OBJECTSREADER_H
#define SHIP_STOWAGE_MODEL_OBJECTSREADER_H

#include "../../data_objects/ShipPlan.h"
#include "../../data_objects/ShipRoute.h"


// TODO: return objects instead of modifying

/**
   * Reads the ship plan from a file. Returns true if finished successfully.
   * @param shipPlan Ship plan that will be modified, should be passed empty.
   */
bool readShipPlanFromFile(const std::string &filePath, ShipPlan &shipPlan);

/// Reads a route file (list of ports).
bool readShipRouteFromFile(const std::string &filePath, ShipRoute &shipRoute);

/// Adds cargo to a port.
bool readCargoToPortFromFile(const std::string &filePath, Port &port);

/// Reads operations from a file.
bool readPackingOperationsFromFile(const std::string &filePath, OPS &operations);

/// Write operations to a file.
bool writePackingOperationsToFile(const std::string &filePath, OPS &operations);


#endif //SHIP_STOWAGE_MODEL_OBJECTSREADER_H
