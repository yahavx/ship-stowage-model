//
// Created by t-yabeny on 4/7/2020.
//

#ifndef SHIP_STOWAGE_MODEL_FILEREADER_H
#define SHIP_STOWAGE_MODEL_FILEREADER_H

#include <string>
#include <vector>
#include "../data_objects/ShipPlan.h"
#include "../utils/Definitions.h"


/// Reads a .csv or .txt file to a 2-dimension matrix, ignores lines that start with '#' and empty lines.
StringStringVector readFile(const std::string &path);



#endif //SHIP_STOWAGE_MODEL_FILEREADER_H
