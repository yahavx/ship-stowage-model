//
// Created by t-yabeny on 4/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_OBJECTSREADER_H
#define SHIP_STOWAGE_MODEL_OBJECTSREADER_H

#include "../data_objects/ShipPlan.h"



/**
   * Reads the ship plan from a file.
   * @param shipPlan Ship plan that will be modified, should be passed empty.
   */
bool readShipPlan(const std::string &filePath, ShipPlan &shipPlan);


#endif //SHIP_STOWAGE_MODEL_OBJECTSREADER_H
