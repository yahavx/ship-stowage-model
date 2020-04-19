//
// Created by t-yabeny on 4/19/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATORUTIL_H
#define SHIP_STOWAGE_MODEL_SIMULATORUTIL_H

#include "../utils/Definitions.h"


/**
 * Receives a travel directory and sorts the .cargo_files for each port.
 * @return a map from each portID to a string vector of his .cargo_files, ordered from small to big.
 */
StringToStringVectorMap sortTravelCargoData(const std::string &directoryPath);  // TODO




#endif //SHIP_STOWAGE_MODEL_SIMULATORUTIL_H
