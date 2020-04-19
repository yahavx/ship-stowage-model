//
// Created by t-yabeny on 4/19/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATORUTIL_H
#define SHIP_STOWAGE_MODEL_SIMULATORUTIL_H

#include "../utils/Definitions.h"
#include "../data_objects/ShipRoute.h"


/**
 * Receives a travel directory and sorts the .cargo_files for each port.
 * @return a map from each portID to a string vector of his .cargo_files, ordered from small to big.
 */
StringToStringVectorMap sortTravelCargoData(const std::string &directoryPath);

/// Returns the next .cargo_data file of the port with portId. Returns null if this port hasn't any cargo_data file remaining.
std::optional<std::string> getNextFileForPort(StringToStringVectorMap &map, const std::string &portId);

/// Removes from map ports that doesn't appear in the route.
void filterUnusedPorts(StringToStringVectorMap &map, const ShipRoute &shipRoute);

/// Removes from route ports that appears twice in a row.
void filterTwiceInARowPorts(ShipRoute &shipRoute);

#endif //SHIP_STOWAGE_MODEL_SIMULATORUTIL_H
